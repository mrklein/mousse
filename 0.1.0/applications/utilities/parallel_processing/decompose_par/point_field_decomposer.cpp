// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_field_decomposer.hpp"
// Constructors 
mousse::pointFieldDecomposer::patchFieldDecomposer::patchFieldDecomposer
(
  const pointPatch& completeMeshPatch,
  const pointPatch& procMeshPatch,
  const labelList& directAddr
)
:
  pointPatchFieldMapperPatchRef
  (
    completeMeshPatch,
    procMeshPatch
  ),
  directAddressing_(procMeshPatch.size(), -1),
  hasUnmapped_(false)
{
  // Create the inverse-addressing of the patch point labels.
  labelList pointMap(completeMeshPatch.boundaryMesh().mesh().size(), -1);
  const labelList& completeMeshPatchPoints = completeMeshPatch.meshPoints();
  forAll(completeMeshPatchPoints, pointi)
  {
    pointMap[completeMeshPatchPoints[pointi]] = pointi;
  }
  // Use the inverse point addressing to create the addressing table for this
  // patch
  const labelList& procMeshPatchPoints = procMeshPatch.meshPoints();
  forAll(procMeshPatchPoints, pointi)
  {
    directAddressing_[pointi] =
      pointMap[directAddr[procMeshPatchPoints[pointi]]];
  }
  // Check that all the patch point addresses are set
  if (directAddressing_.size() && min(directAddressing_) < 0)
  {
    hasUnmapped_ = true;
    FatalErrorIn
    (
      "pointFieldDecomposer::patchFieldDecomposer()"
    )   << "Incomplete patch point addressing"
      << abort(FatalError);
  }
}
mousse::pointFieldDecomposer::pointFieldDecomposer
(
  const pointMesh& completeMesh,
  const pointMesh& procMesh,
  const labelList& pointAddressing,
  const labelList& boundaryAddressing
)
:
  completeMesh_(completeMesh),
  procMesh_(procMesh),
  pointAddressing_(pointAddressing),
  boundaryAddressing_(boundaryAddressing),
  patchFieldDecomposerPtrs_
  (
    procMesh_.boundary().size(),
    static_cast<patchFieldDecomposer*>(NULL)
  )
{
  forAll(boundaryAddressing_, patchi)
  {
    if (boundaryAddressing_[patchi] >= 0)
    {
      patchFieldDecomposerPtrs_[patchi] = new patchFieldDecomposer
      (
        completeMesh_.boundary()[boundaryAddressing_[patchi]],
        procMesh_.boundary()[patchi],
        pointAddressing_
      );
    }
  }
}
// Destructor 
mousse::pointFieldDecomposer::~pointFieldDecomposer()
{
  forAll(patchFieldDecomposerPtrs_, patchi)
  {
    if (patchFieldDecomposerPtrs_[patchi])
    {
      delete patchFieldDecomposerPtrs_[patchi];
    }
  }
}
