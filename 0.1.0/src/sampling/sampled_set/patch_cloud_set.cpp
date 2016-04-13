// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_cloud_set.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tree_bound_box.hpp"
#include "tree_data_face.hpp"
#include "time.hpp"
#include "mesh_tools.hpp"
// For 'nearInfo' helper class only
#include "mapped_patch_base.hpp"
#include "ofstream.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(patchCloudSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, patchCloudSet, word);

}


// Private Member Functions 
void mousse::patchCloudSet::calcSamples
(
  DynamicList<point>& samplingPts,
  DynamicList<label>& samplingCells,
  DynamicList<label>& samplingFaces,
  DynamicList<label>& samplingSegments,
  DynamicList<scalar>& samplingCurveDist
) const
{
  if (debug) {
    Info << "patchCloudSet : sampling on patches :" << endl;
  }
  // Construct search tree for all patch faces.
  label sz = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter) {
    const polyPatch& pp = mesh().boundaryMesh()[iter.key()];
    sz += pp.size();
    if (debug) {
      Info << "    " << pp.name() << " size " << pp.size() << endl;
    }
  }
  labelList patchFaces{sz};
  sz = 0;
  treeBoundBox bb{point::max, point::min};
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter) {
    const polyPatch& pp = mesh().boundaryMesh()[iter.key()];
    FOR_ALL(pp, i) {
      patchFaces[sz++] = pp.start()+i;
    }
    // Do not do reduction.
    const boundBox patchBb(pp.points(), pp.meshPoints(), false);
    bb.min() = min(bb.min(), patchBb.min());
    bb.max() = max(bb.max(), patchBb.max());
  }
  // Not very random
  Random rndGen{123456};
  // Make bb asymetric just to avoid problems on symmetric meshes
  bb = bb.extend(rndGen, 1e-4);
  // Make sure bb is 3D.
  bb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  bb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  indexedOctree<treeDataFace> patchTree
  {
    // all information needed to search faces
    {
      false,      // do not cache bb
      mesh(),
      patchFaces  // boundary faces only
    },
    bb,             // overall search domain
    8,              // maxLevel
    10,             // leafsize
    3.0             // duplicity
  };
  // Force calculation of face-diagonal decomposition
  (void)mesh().tetBasePtIs();
  // All the info for nearest. Construct to miss
  List<mappedPatchBase::nearInfo> nearest{sampleCoords_.size()};
  FOR_ALL(sampleCoords_, sampleI) {
    const point& sample = sampleCoords_[sampleI];
    pointIndexHit& nearInfo = nearest[sampleI].first();
    // Find the nearest locally
    if (patchFaces.size()) {
      nearInfo = patchTree.findNearest(sample, sqr(searchDist_));
    } else {
      nearInfo.setMiss();
    }
    // Fill in the distance field and the processor field
    if (!nearInfo.hit()) {
      nearest[sampleI].second().first() = mousse::sqr(GREAT);
      nearest[sampleI].second().second() = Pstream::myProcNo();
    } else {
      // Set nearest to mesh face label
      nearInfo.setIndex(patchFaces[nearInfo.index()]);
      nearest[sampleI].second().first() = magSqr(nearInfo.hitPoint() - sample);
      nearest[sampleI].second().second() = Pstream::myProcNo();
    }
  }
  // Find nearest.
  Pstream::listCombineGather(nearest, mappedPatchBase::nearestEqOp());
  Pstream::listCombineScatter(nearest);
  if (debug && Pstream::master()) {
    OFstream str{mesh().time().path()/ name() + "_nearest.obj"};
    Info << "Dumping mapping as lines from supplied points to"
      << " nearest patch face to file " << str.name() << endl;
    label vertI = 0;
    FOR_ALL(nearest, i) {
      if (!nearest[i].first().hit())
        continue;
      meshTools::writeOBJ(str, sampleCoords_[i]);
      vertI++;
      meshTools::writeOBJ(str, nearest[i].first().hitPoint());
      vertI++;
      str << "l " << vertI-1 << ' ' << vertI << nl;
    }
  }
  // Store the sampling locations on the nearest processor
  FOR_ALL(nearest, sampleI) {
    const pointIndexHit& nearInfo = nearest[sampleI].first();
    if (nearInfo.hit()) {
      if (nearest[sampleI].second().second() == Pstream::myProcNo())
      {
        label faceI = nearInfo.index();
        samplingPts.append(nearInfo.hitPoint());
        samplingCells.append(mesh().faceOwner()[faceI]);
        samplingFaces.append(faceI);
        samplingSegments.append(0);
        samplingCurveDist.append(1.0 * sampleI);
      }
    } else {
      // No processor found point near enough. Mark with special value
      // which is intercepted when interpolating
      if (Pstream::master()) {
        samplingPts.append(sampleCoords_[sampleI]);
        samplingCells.append(-1);
        samplingFaces.append(-1);
        samplingSegments.append(0);
        samplingCurveDist.append(1.0 * sampleI);
      }
    }
  }
}


void mousse::patchCloudSet::genSamples()
{
  // Storage for sample points
  DynamicList<point> samplingPts;
  DynamicList<label> samplingCells;
  DynamicList<label> samplingFaces;
  DynamicList<label> samplingSegments;
  DynamicList<scalar> samplingCurveDist;
  calcSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
  samplingPts.shrink();
  samplingCells.shrink();
  samplingFaces.shrink();
  samplingSegments.shrink();
  samplingCurveDist.shrink();
  setSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
}


// Constructors 
mousse::patchCloudSet::patchCloudSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis,
  const List<point>& sampleCoords,
  const labelHashSet& patchSet,
  const scalar searchDist
)
:
  sampledSet{name, mesh, searchEngine, axis},
  sampleCoords_{sampleCoords},
  patchSet_{patchSet},
  searchDist_{searchDist}
{
  genSamples();
  if (debug) {
    write(Info);
  }
}


mousse::patchCloudSet::patchCloudSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  sampledSet{name, mesh, searchEngine, dict},
  sampleCoords_{dict.lookup("points")},
  patchSet_
  {
    mesh.boundaryMesh().patchSet(wordReList{dict.lookup("patches")})
  },
  searchDist_{readScalar(dict.lookup("maxDistance"))}
{
  genSamples();
  if (debug) {
    write(Info);
  }
}


// Destructor 
mousse::patchCloudSet::~patchCloudSet()
{}

