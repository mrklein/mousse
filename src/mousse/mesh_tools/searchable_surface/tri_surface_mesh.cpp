// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_mesh.hpp"
#include "random.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "edge_map.hpp"
#include "tri_surface_fields.hpp"
#include "time.hpp"
#include "patch_tools.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(triSurfaceMesh, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(searchableSurface, triSurfaceMesh, dict);

}


// Private Member Functions 

//- Check file existence
const mousse::fileName& mousse::triSurfaceMesh::checkFile
(
  const fileName& fName,
  const fileName& objectName
)
{
  if (fName.empty()) {
    FATAL_ERROR_IN
    (
      "triSurfaceMesh::checkFile(const fileName&, const fileName&)"
    )
    << "Cannot find triSurfaceMesh starting from "
    << objectName << exit(FatalError);
  }
  return fName;
}


bool mousse::triSurfaceMesh::addFaceToEdge
(
  const edge& e,
  EdgeMap<label>& facesPerEdge
)
{
  EdgeMap<label>::iterator eFnd = facesPerEdge.find(e);
  if (eFnd != facesPerEdge.end()) {
    if (eFnd() == 2) {
      return false;
    }
    eFnd()++;
  } else {
    facesPerEdge.insert(e, 1);
  }
  return true;
}


bool mousse::triSurfaceMesh::isSurfaceClosed() const
{
  const pointField& pts = triSurface::points();
  // Construct pointFaces. Let's hope surface has compact point
  // numbering ...
  labelListList pointFaces;
  invertManyToMany(pts.size(), *this, pointFaces);
  // Loop over all faces surrounding point. Count edges emanating from point.
  // Every edge should be used by two faces exactly.
  // To prevent doing work twice per edge only look at edges to higher
  // point
  EdgeMap<label> facesPerEdge{100};
  FOR_ALL(pointFaces, pointI) {
    const labelList& pFaces = pointFaces[pointI];
    facesPerEdge.clear();
    FOR_ALL(pFaces, i) {
      const triSurface::FaceType& f = triSurface::operator[](pFaces[i]);
      label fp = findIndex(f, pointI);
      // Something weird: if I expand the code of addFaceToEdge in both
      // below instances it gives a segmentation violation on some
      // surfaces. Compiler (4.3.2) problem?
      // Forward edge
      label nextPointI = f[f.fcIndex(fp)];
      if (nextPointI > pointI) {
        bool okFace = addFaceToEdge
        (
          edge(pointI, nextPointI),
          facesPerEdge
        );
        if (!okFace) {
          return false;
        }
      }
      // Reverse edge
      label prevPointI = f[f.rcIndex(fp)];
      if (prevPointI > pointI) {
        bool okFace = addFaceToEdge
        (
          edge(pointI, prevPointI),
          facesPerEdge
        );
        if (!okFace) {
          return false;
        }
      }
    }
    // Check for any edges used only once.
    FOR_ALL_CONST_ITER(EdgeMap<label>, facesPerEdge, iter) {
      if (iter() != 2) {
        return false;
      }
    }
  }
  return true;
}


// Constructors 
mousse::triSurfaceMesh::triSurfaceMesh(const IOobject& io, const triSurface& s)
:
  searchableSurface{io},
  objectRegistry
  {
    {
      io.name(),
      io.instance(),
      io.local(),
      io.db(),
      io.readOpt(),
      io.writeOpt(),
      false       // searchableSurface already registered under name
    }
  },
  triSurface{s},
  triSurfaceRegionSearch{s},
  minQuality_{-1},
  surfaceClosed_{-1}
{
  const pointField& pts = triSurface::points();
  bounds() = boundBox(pts);
}


mousse::triSurfaceMesh::triSurfaceMesh(const IOobject& io)
:
  // Find instance for triSurfaceMesh
  searchableSurface{io},
  // Reused found instance in objectRegistry
  objectRegistry
  {
    {
      io.name(),
      static_cast<const searchableSurface&>(*this).instance(),
      io.local(),
      io.db(),
      io.readOpt(),
      io.writeOpt(),
      false       // searchableSurface already registered under name
    }
  },
  triSurface
  {
    checkFile
    (
      searchableSurface::filePath(),
      searchableSurface::objectPath()
    )
  },
  triSurfaceRegionSearch{static_cast<const triSurface&>(*this)},
  minQuality_{-1},
  surfaceClosed_{-1}
{
  const pointField& pts = triSurface::points();
  bounds() = boundBox(pts);
}


mousse::triSurfaceMesh::triSurfaceMesh
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface{io},
  // Reused found instance in objectRegistry
  objectRegistry
  {
    {
      io.name(),
      static_cast<const searchableSurface&>(*this).instance(),
      io.local(),
      io.db(),
      io.readOpt(),
      io.writeOpt(),
      false       // searchableSurface already registered under name
    }
  },
  triSurface
  {
    checkFile
    (
      searchableSurface::filePath(),
      searchableSurface::objectPath()
    )
  },
  triSurfaceRegionSearch{static_cast<const triSurface&>(*this), dict},
  minQuality_{-1},
  surfaceClosed_{-1}
{
  scalar scaleFactor = 0;
  // allow rescaling of the surface points
  // eg, CAD geometries are often done in millimeters
  if (dict.readIfPresent("scale", scaleFactor) && scaleFactor > 0) {
    Info << searchableSurface::name() << " : using scale " << scaleFactor
      << endl;
    triSurface::scalePoints(scaleFactor);
  }
  const pointField& pts = triSurface::points();
  bounds() = boundBox(pts);
  // Have optional minimum quality for normal calculation
  if (dict.readIfPresent("minQuality", minQuality_) && minQuality_ > 0) {
    Info << searchableSurface::name()
      << " : ignoring triangles with quality < "
      << minQuality_ << " for normals calculation." << endl;
  }
}


// Destructor 
mousse::triSurfaceMesh::~triSurfaceMesh()
{
  clearOut();
}


void mousse::triSurfaceMesh::clearOut()
{
  triSurfaceRegionSearch::clearOut();
  edgeTree_.clear();
  triSurface::clearOut();
}


// Member Functions 
mousse::tmp<mousse::pointField> mousse::triSurfaceMesh::coordinates() const
{
  tmp<pointField> tPts{new pointField{8}};
  pointField& pt = tPts();
  // Use copy to calculate face centres so they don't get stored
  pt = PrimitivePatch<triSurface::FaceType, SubList, const pointField&>
  (
    SubList<triSurface::FaceType>(*this, triSurface::size()),
    triSurface::points()
  ).faceCentres();
  return tPts;
}


void mousse::triSurfaceMesh::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres = coordinates();
  radiusSqr.setSize(size());
  radiusSqr = 0.0;
  const pointField& pts = triSurface::points();
  FOR_ALL(*this, faceI) {
    const labelledTri& f = triSurface::operator[](faceI);
    const point& fc = centres[faceI];
    FOR_ALL(f, fp) {
      const point& pt = pts[f[fp]];
      radiusSqr[faceI] = max(radiusSqr[faceI], mousse::magSqr(fc-pt));
    }
  }
  // Add a bit to make sure all points are tested inside
  radiusSqr += mousse::sqr(SMALL);
}


mousse::tmp<mousse::pointField> mousse::triSurfaceMesh::points() const
{
  return triSurface::points();
}


bool mousse::triSurfaceMesh::overlaps(const boundBox& bb) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  labelList indices = octree.findBox(treeBoundBox(bb));
  return !indices.empty();
}


void mousse::triSurfaceMesh::movePoints(const pointField& newPoints)
{
  triSurfaceRegionSearch::clearOut();
  edgeTree_.clear();
  triSurface::movePoints(newPoints);
}


const mousse::indexedOctree<mousse::treeDataEdge>&
mousse::triSurfaceMesh::edgeTree() const
{
  if (edgeTree_.empty()) {
    // Boundary edges
    labelList bEdges
    {
      identity(nEdges() - nInternalEdges()) + nInternalEdges()
    };
    treeBoundBox bb{vector::zero, vector::zero};
    if (bEdges.size()) {
      label nPoints;
      PatchTools::calcBounds
      (
        static_cast<const triSurface&>(*this),
        bb,
        nPoints
      );
      // Random number generator. Bit dodgy since not exactly random ;-)
      Random rndGen{65431};
      // Slightly extended bb. Slightly off-centred just so on symmetric
      // geometry there are less face/edge aligned items.
      bb = bb.extend(rndGen, 1e-4);
      bb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
      bb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    }
    scalar oldTol = indexedOctree<treeDataEdge>::perturbTol();
    indexedOctree<treeDataEdge>::perturbTol() = tolerance();
    edgeTree_.reset
    (
      new indexedOctree<treeDataEdge>
      {
        treeDataEdge
        {
          false,          // cachebb
          edges(),        // edges
          localPoints(),  // points
          bEdges          // selected edges
        },
        bb,                 // bb
        maxTreeDepth(),      // maxLevel
        10,                 // leafsize
        3.0                 // duplicity
      }
    );
    indexedOctree<treeDataEdge>::perturbTol() = oldTol;
  }
  return edgeTree_();
}


const mousse::wordList& mousse::triSurfaceMesh::regions() const
{
  if (regions_.empty()) {
    regions_.setSize(patches().size());
    FOR_ALL(regions_, regionI) {
      regions_[regionI] = patches()[regionI].name();
    }
  }
  return regions_;
}


// Find out if surface is closed.
bool mousse::triSurfaceMesh::hasVolumeType() const
{
  if (surfaceClosed_ == -1) {
    if (isSurfaceClosed()) {
      surfaceClosed_ = 1;
    } else {
      surfaceClosed_ = 0;
    }
  }
  return surfaceClosed_ == 1;
}


void mousse::triSurfaceMesh::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  triSurfaceSearch::findNearest(samples, nearestDistSqr, info);
}


void mousse::triSurfaceMesh::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  const labelList& regionIndices,
  List<pointIndexHit>& info
) const
{
  triSurfaceRegionSearch::findNearest
  (
    samples,
    nearestDistSqr,
    regionIndices,
    info
  );
}


void mousse::triSurfaceMesh::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  triSurfaceSearch::findLine(start, end, info);
}


void mousse::triSurfaceMesh::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  triSurfaceSearch::findLineAny(start, end, info);
}


void mousse::triSurfaceMesh::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  triSurfaceSearch::findLineAll(start, end, info);
}


void mousse::triSurfaceMesh::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  FOR_ALL(info, i) {
    if (info[i].hit()) {
      region[i] = triSurface::operator[](info[i].index()).region();
    } else {
      region[i] = -1;
    }
  }
}


void mousse::triSurfaceMesh::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  const triSurface& s = static_cast<const triSurface&>(*this);
  const pointField& pts = s.points();
  normal.setSize(info.size());
  if (minQuality_ >= 0) {
    // Make sure we don't use triangles with low quality since
    // normal is not reliable.
    const labelListList& faceFaces = s.faceFaces();
    FOR_ALL(info, i) {
      if (info[i].hit()) {
        label faceI = info[i].index();
        normal[i] = s[faceI].normal(pts);
        scalar qual = s[faceI].tri(pts).quality();
        if (qual < minQuality_) {
          // Search neighbouring triangles
          const labelList& fFaces = faceFaces[faceI];
          FOR_ALL(fFaces, j) {
            label nbrI = fFaces[j];
            scalar nbrQual = s[nbrI].tri(pts).quality();
            if (nbrQual > qual) {
              qual = nbrQual;
              normal[i] = s[nbrI].normal(pts);
            }
          }
        }
        normal[i] /= mag(normal[i]) + VSMALL;
      } else {
        // Set to what?
        normal[i] = vector::zero;
      }
    }
  } else {
    FOR_ALL(info, i) {
      if (info[i].hit()) {
        label faceI = info[i].index();
        //- Cached:
        //normal[i] = faceNormals()[faceI];
        //- Uncached
        normal[i] = s[faceI].normal(pts);
        normal[i] /= mag(normal[i]) + VSMALL;
      } else {
        // Set to what?
        normal[i] = vector::zero;
      }
    }
  }
}


void mousse::triSurfaceMesh::setField(const labelList& values)
{
  autoPtr<triSurfaceLabelField> fldPtr
  {
    new triSurfaceLabelField
    {
      {
        "values",
        objectRegistry::time().timeName(),  // instance
        "triSurface",                       // local
        *this,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      *this,
      dimless,
      labelField(values)
    }
  };
  // Store field on triMesh
  fldPtr.ptr()->store();
}


void mousse::triSurfaceMesh::getField
(
  const List<pointIndexHit>& info,
  labelList& values
) const
{
  if (foundObject<triSurfaceLabelField>("values")) {
    values.setSize(info.size());
    const triSurfaceLabelField& fld = lookupObject<triSurfaceLabelField>
    (
      "values"
    );
    FOR_ALL(info, i) {
      if (info[i].hit()) {
        values[i] = fld[info[i].index()];
      }
    }
  }
}


void mousse::triSurfaceMesh::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  volType.setSize(points.size());
  scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
  indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();
  FOR_ALL(points, pointI) {
    const point& pt = points[pointI];
    if (!tree().bb().contains(pt)) {
      // Have to calculate directly as outside the octree
      volType[pointI] = tree().shapes().getVolumeType(tree(), pt);
    } else {
      // - use cached volume type per each tree node
      volType[pointI] = tree().getVolumeType(pt);
    }
  }
  indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}


//- Write using given format, version and compression
bool mousse::triSurfaceMesh::writeObject
(
  IOstream::streamFormat /*fmt*/,
  IOstream::versionNumber /*ver*/,
  IOstream::compressionType /*cmp*/
) const
{
  fileName fullPath(searchableSurface::objectPath());
  if (!mkDir(fullPath.path())) {
    return false;
  }
  triSurface::write(fullPath);
  if (!isFile(fullPath)) {
    return false;
  }
  return true;
}

