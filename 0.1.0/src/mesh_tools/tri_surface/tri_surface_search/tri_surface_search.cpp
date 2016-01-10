// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_search.hpp"
#include "tri_surface.hpp"
#include "patch_tools.hpp"
#include "volume_type.hpp"
#include "map.hpp"
#include "dictionary.hpp"

// Private Member Functions
bool mousse::triSurfaceSearch::checkUniqueHit
(
  const pointIndexHit& currHit,
  const DynamicList<pointIndexHit, 1, 1>& hits,
  const vector& lineVec
) const
{
  // Classify the hit
  label nearType = -1;
  label nearLabel = -1;
  const labelledTri& f = surface()[currHit.index()];
  f.nearestPointClassify
  (
    currHit.hitPoint(),
    surface().points(),
    nearType,
    nearLabel
  );
  if (nearType == triPointRef::POINT)
  {
    // near point
    const label nearPointI = f[nearLabel];
    const labelList& pointFaces =
      surface().pointFaces()[surface().meshPointMap()[nearPointI]];
    FOR_ALL(pointFaces, pI)
    {
      const label pointFaceI = pointFaces[pI];
      if (pointFaceI != currHit.index())
      {
        FOR_ALL(hits, hI)
        {
          const pointIndexHit& hit = hits[hI];
          if (hit.index() == pointFaceI)
          {
            return false;
          }
        }
      }
    }
  }
  else if (nearType == triPointRef::EDGE)
  {
    // near edge
    // check if the other face of the edge is already hit
    const labelList& fEdges = surface().faceEdges()[currHit.index()];
    const label edgeI = fEdges[nearLabel];
    const labelList& edgeFaces = surface().edgeFaces()[edgeI];
    FOR_ALL(edgeFaces, fI)
    {
      const label edgeFaceI = edgeFaces[fI];
      if (edgeFaceI != currHit.index())
      {
        FOR_ALL(hits, hI)
        {
          const pointIndexHit& hit = hits[hI];
          if (hit.index() == edgeFaceI)
          {
            // Check normals
            const vector currHitNormal =
              surface().faceNormals()[currHit.index()];
            const vector existingHitNormal =
              surface().faceNormals()[edgeFaceI];
            const label signCurrHit =
              pos(currHitNormal & lineVec);
            const label signExistingHit =
              pos(existingHitNormal & lineVec);
            if (signCurrHit == signExistingHit)
            {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}
// Constructors
mousse::triSurfaceSearch::triSurfaceSearch(const triSurface& surface)
:
  surface_(surface),
  tolerance_(indexedOctree<treeDataTriSurface>::perturbTol()),
  maxTreeDepth_(10),
  treePtr_(NULL)
{}
mousse::triSurfaceSearch::triSurfaceSearch
(
  const triSurface& surface,
  const dictionary& dict
)
:
  surface_(surface),
  tolerance_(indexedOctree<treeDataTriSurface>::perturbTol()),
  maxTreeDepth_(10),
  treePtr_(NULL)
{
  // Have optional non-standard search tolerance for gappy surfaces.
  if (dict.readIfPresent("tolerance", tolerance_) && tolerance_ > 0)
  {
    Info<< "    using intersection tolerance " << tolerance_ << endl;
  }
  // Have optional non-standard tree-depth to limit storage.
  if (dict.readIfPresent("maxTreeDepth", maxTreeDepth_) && maxTreeDepth_ > 0)
  {
    Info<< "    using maximum tree depth " << maxTreeDepth_ << endl;
  }
}
mousse::triSurfaceSearch::triSurfaceSearch
(
  const triSurface& surface,
  const scalar tolerance,
  const label maxTreeDepth
)
:
  surface_(surface),
  tolerance_(tolerance),
  maxTreeDepth_(maxTreeDepth),
  treePtr_(NULL)
{}

// Destructor
mousse::triSurfaceSearch::~triSurfaceSearch()
{
  clearOut();
}
void mousse::triSurfaceSearch::clearOut()
{
  treePtr_.clear();
}

// Member Functions
const mousse::indexedOctree<mousse::treeDataTriSurface>&
mousse::triSurfaceSearch::tree() const
{
  if (treePtr_.empty())
  {
    // Calculate bb without constructing local point numbering.
    treeBoundBox bb(vector::zero, vector::zero);
    if (surface().size())
    {
      label nPoints;
      PatchTools::calcBounds(surface(), bb, nPoints);
      if (nPoints != surface().points().size())
      {
        WARNING_IN("triSurfaceSearch::tree() const")
          << "Surface does not have compact point numbering."
          << " Of " << surface().points().size()
          << " only " << nPoints
          << " are used. This might give problems in some routines."
          << endl;
      }
      // Random number generator. Bit dodgy since not exactly random ;-)
      Random rndGen(65431);
      // Slightly extended bb. Slightly off-centred just so on symmetric
      // geometry there are less face/edge aligned items.
      bb = bb.extend(rndGen, 1e-4);
      bb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
      bb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    }
    scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
    indexedOctree<treeDataTriSurface>::perturbTol() = tolerance_;
    treePtr_.reset
    (
      new indexedOctree<treeDataTriSurface>
      (
        treeDataTriSurface(true, surface_, tolerance_),
        bb,
        maxTreeDepth_,  // maxLevel
        10,             // leafsize
        3.0             // duplicity
      )
    );
    indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
  }
  return treePtr_();
}
// Determine inside/outside for samples
mousse::boolList mousse::triSurfaceSearch::calcInside
(
  const pointField& samples
) const
{
  boolList inside(samples.size());
  FOR_ALL(samples, sampleI)
  {
    const point& sample = samples[sampleI];
    if (!tree().bb().contains(sample))
    {
      inside[sampleI] = false;
    }
    else if (tree().getVolumeType(sample) == volumeType::INSIDE)
    {
      inside[sampleI] = true;
    }
    else
    {
      inside[sampleI] = false;
    }
  }
  return inside;
}
void mousse::triSurfaceSearch::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
  indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();
  const indexedOctree<treeDataTriSurface>& octree = tree();
  info.setSize(samples.size());
  FOR_ALL(samples, i)
  {
    static_cast<pointIndexHit&>(info[i]) = octree.findNearest
    (
      samples[i],
      nearestDistSqr[i],
      treeDataTriSurface::findNearestOp(octree)
    );
  }
  indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}
mousse::pointIndexHit mousse::triSurfaceSearch::nearest
(
  const point& pt,
  const vector& span
)
const
{
  const scalar nearestDistSqr = 0.25*magSqr(span);
  return tree().findNearest(pt, nearestDistSqr);
}
void mousse::triSurfaceSearch::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  info.setSize(start.size());
  scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
  indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();
  FOR_ALL(start, i)
  {
    static_cast<pointIndexHit&>(info[i]) = octree.findLine
    (
      start[i],
      end[i]
    );
  }
  indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}
void mousse::triSurfaceSearch::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  info.setSize(start.size());
  scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
  indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();
  FOR_ALL(start, i)
  {
    static_cast<pointIndexHit&>(info[i]) = octree.findLineAny
    (
      start[i],
      end[i]
    );
  }
  indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}
void mousse::triSurfaceSearch::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  info.setSize(start.size());
  scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
  indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();
  // Work array
  DynamicList<pointIndexHit, 1, 1> hits;
  DynamicList<label> shapeMask;
  treeDataTriSurface::findAllIntersectOp allIntersectOp(octree, shapeMask);
  FOR_ALL(start, pointI)
  {
    hits.clear();
    shapeMask.clear();
    while (true)
    {
      // See if any intersection between pt and end
      pointIndexHit inter = octree.findLine
      (
        start[pointI],
        end[pointI],
        allIntersectOp
      );
      if (inter.hit())
      {
        vector lineVec = end[pointI] - start[pointI];
        lineVec /= mag(lineVec) + VSMALL;
        if
        (
          checkUniqueHit
          (
            inter,
            hits,
            lineVec
          )
        )
        {
          hits.append(inter);
        }
        shapeMask.append(inter.index());
      }
      else
      {
        break;
      }
    }
    info[pointI].transfer(hits);
  }
  indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}
