// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_field_reconstructor.hpp"
// Constructors 
mousse::fvFieldReconstructor::fvFieldReconstructor
(
  fvMesh& mesh,
  const PtrList<fvMesh>& procMeshes,
  const PtrList<labelIOList>& faceProcAddressing,
  const PtrList<labelIOList>& cellProcAddressing,
  const PtrList<labelIOList>& boundaryProcAddressing
)
:
  mesh_(mesh),
  procMeshes_(procMeshes),
  faceProcAddressing_(faceProcAddressing),
  cellProcAddressing_(cellProcAddressing),
  boundaryProcAddressing_(boundaryProcAddressing),
  nReconstructed_(0)
{
  forAll(procMeshes_, procI)
  {
    const fvMesh& procMesh = procMeshes_[procI];
    if
    (
      faceProcAddressing[procI].size() != procMesh.nFaces()
    || cellProcAddressing[procI].size() != procMesh.nCells()
    || boundaryProcAddressing[procI].size() != procMesh.boundary().size()
    )
    {
      FatalErrorIn
      (
        "fvFieldReconstructor::fvFieldReconstructor\n"
        "(\n"
        "   fvMesh&,\n"
        "   const PtrList<fvMesh>&,\n"
        "   const PtrList<labelIOList>&,\n"
        "   const PtrList<labelIOList>&,\n"
        "   const PtrList<labelIOList>&\n"
        ")"
      )   << "Size of maps does not correspond to size of mesh"
        << " for processor " << procI << endl
        << "faceProcAddressing : " << faceProcAddressing[procI].size()
        << " nFaces : " << procMesh.nFaces() << endl
        << "cellProcAddressing : " << cellProcAddressing[procI].size()
        << " nCell : " << procMesh.nCells() << endl
        << "boundaryProcAddressing : "
        << boundaryProcAddressing[procI].size()
        << " nFaces : " << procMesh.boundary().size()
        << exit(FatalError);
    }
  }
}
