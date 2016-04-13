// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_boundary_mesh.hpp"
#include "fv_mesh.hpp"


// Private Member Functions 
void mousse::fvBoundaryMesh::addPatches(const polyBoundaryMesh& basicBdry)
{
  setSize(basicBdry.size());
  // Set boundary patches
  fvPatchList& Patches = *this;
  FOR_ALL(Patches, patchI) {
    Patches.set(patchI, fvPatch::New(basicBdry[patchI], *this));
  }
}


// Constructors 
mousse::fvBoundaryMesh::fvBoundaryMesh
(
  const fvMesh& m
)
:
  fvPatchList{0},
  mesh_{m}
{}


mousse::fvBoundaryMesh::fvBoundaryMesh
(
  const fvMesh& m,
  const polyBoundaryMesh& basicBdry
)
:
  fvPatchList{basicBdry.size()},
  mesh_{m}
{
  addPatches(basicBdry);
}


// Member Functions 
mousse::label mousse::fvBoundaryMesh::findPatchID(const word& patchName) const
{
  const fvPatchList& patches = *this;
  FOR_ALL(patches, patchI) {
    if (patches[patchI].name() == patchName) {
      return patchI;
    }
  }
  // Not found, return -1
  return -1;
}


mousse::labelList mousse::fvBoundaryMesh::findIndices
(
  const keyType& key,
  const bool usePatchGroups
) const
{
  return mesh().boundaryMesh().findIndices(key, usePatchGroups);
}


void mousse::fvBoundaryMesh::movePoints()
{
  FOR_ALL(*this, patchI) {
    operator[](patchI).initMovePoints();
  }
  FOR_ALL(*this, patchI) {
    operator[](patchI).movePoints();
  }
}


mousse::lduInterfacePtrsList mousse::fvBoundaryMesh::interfaces() const
{
  lduInterfacePtrsList interfaces(size());
  FOR_ALL(interfaces, patchI) {
    if (isA<lduInterface>(this->operator[](patchI))) {
      interfaces.set
      (
        patchI,
        &refCast<const lduInterface>(this->operator[](patchI))
      );
    }
  }
  return interfaces;
}


void mousse::fvBoundaryMesh::readUpdate(const polyBoundaryMesh& basicBdry)
{
  clear();
  addPatches(basicBdry);
}


// Member Operators 
const mousse::fvPatch& mousse::fvBoundaryMesh::operator[]
(
  const word& patchName
) const
{
  const label patchI = findPatchID(patchName);
  if (patchI < 0) {
    FATAL_ERROR_IN
    (
      "fvBoundaryMesh::operator[](const word&) const"
    )
    << "Patch named " << patchName << " not found." << nl
    << abort(FatalError);
  }
  return operator[](patchI);
}


mousse::fvPatch& mousse::fvBoundaryMesh::operator[]
(
  const word& patchName
)
{
  const label patchI = findPatchID(patchName);
  if (patchI < 0) {
    FATAL_ERROR_IN
    (
      "fvBoundaryMesh::operator[](const word&)"
    )
    << "Patch named " << patchName << " not found." << nl
    << abort(FatalError);
  }
  return operator[](patchI);
}

