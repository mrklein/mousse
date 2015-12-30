// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_to_face.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "mapped_patch_base.hpp"
#include "indirect_primitive_patch.hpp"
#include "patch_tools.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "patch_edge_face_wave.hpp"
#include "patch_edge_face_region.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(regionToFace, 0);
addToRunTimeSelectionTable(topoSetSource, regionToFace, word);
addToRunTimeSelectionTable(topoSetSource, regionToFace, istream);
}
mousse::topoSetSource::addToUsageTable mousse::regionToFace::usage_
(
  regionToFace::typeName,
  "\n    Usage: regionToFace <faceSet> (x y z)\n\n"
  "    Select all faces in the connected region of the faceSet"
  " starting from the point.\n"
);
// Private Member Functions 
void mousse::regionToFace::markZone
(
  const indirectPrimitivePatch& patch,
  const label procI,
  const label faceI,
  const label zoneI,
  labelList& faceZone
) const
{
  // Data on all edges and faces
  List<patchEdgeFaceRegion> allEdgeInfo(patch.nEdges());
  List<patchEdgeFaceRegion> allFaceInfo(patch.size());
  DynamicList<label> changedEdges;
  DynamicList<patchEdgeFaceRegion> changedInfo;
  if (Pstream::myProcNo() == procI)
  {
    const labelList& fEdges = patch.faceEdges()[faceI];
    forAll(fEdges, i)
    {
      changedEdges.append(fEdges[i]);
      changedInfo.append(zoneI);
    }
  }
  // Walk
  PatchEdgeFaceWave
  <
    indirectPrimitivePatch,
    patchEdgeFaceRegion
  > calc
  (
    mesh_,
    patch,
    changedEdges,
    changedInfo,
    allEdgeInfo,
    allFaceInfo,
    returnReduce(patch.nEdges(), sumOp<label>())
  );
  forAll(allFaceInfo, faceI)
  {
    if (allFaceInfo[faceI].region() == zoneI)
    {
      faceZone[faceI] = zoneI;
    }
  }
}
void mousse::regionToFace::combine(topoSet& set, const bool add) const
{
  Info<< "    Loading subset " << setName_ << " to delimit search region."
    << endl;
  faceSet subSet(mesh_, setName_);
  indirectPrimitivePatch patch
  (
    IndirectList<face>(mesh_.faces(), subSet.toc()),
    mesh_.points()
  );
  mappedPatchBase::nearInfo ni
  (
    pointIndexHit(false, vector::zero, -1),
    Tuple2<scalar, label>
    (
      sqr(GREAT),
      Pstream::myProcNo()
    )
  );
  forAll(patch, i)
  {
    const point& fc = patch.faceCentres()[i];
    scalar d2 = magSqr(fc-nearPoint_);
    if (!ni.first().hit() || d2 < ni.second().first())
    {
      ni.second().first() = d2;
      ni.first().setHit();
      ni.first().setPoint(fc);
      ni.first().setIndex(i);
    }
  }
  // Globally reduce
  combineReduce(ni, mappedPatchBase::nearestEqOp());
  Info<< "    Found nearest face at " << ni.first().rawPoint()
    << " on processor " << ni.second().second()
    << " face " << ni.first().index()
    << " distance " << mousse::sqrt(ni.second().first()) << endl;
  labelList faceRegion(patch.size(), -1);
  markZone
  (
    patch,
    ni.second().second(),   // procI
    ni.first().index(),     // start face
    0,                      // currentZone
    faceRegion
  );
  forAll(faceRegion, faceI)
  {
    if (faceRegion[faceI] == 0)
    {
      addOrDelete(set, patch.addressing()[faceI], add);
    }
  }
}
// Constructors 
// Construct from components
mousse::regionToFace::regionToFace
(
  const polyMesh& mesh,
  const word& setName,
  const point& nearPoint
)
:
  topoSetSource(mesh),
  setName_(setName),
  nearPoint_(nearPoint)
{}
// Construct from dictionary
mousse::regionToFace::regionToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set")),
  nearPoint_(dict.lookup("nearPoint"))
{}
// Construct from Istream
mousse::regionToFace::regionToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is)),
  nearPoint_(checkIs(is))
{}
// Destructor 
mousse::regionToFace::~regionToFace()
{}
// Member Functions 
void mousse::regionToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all faces of connected region of set "
      << setName_
      << " starting from point "
      << nearPoint_ << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all cells of connected region of set "
      << setName_
      << " starting from point "
      << nearPoint_ << " ..." << endl;
    combine(set, false);
  }
}
