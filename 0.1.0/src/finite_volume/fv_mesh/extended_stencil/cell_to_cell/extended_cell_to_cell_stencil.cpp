// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_cell_to_cell_stencil.hpp"
// Constructors 
mousse::extendedCellToCellStencil::extendedCellToCellStencil(const polyMesh& mesh)
:
  mesh_(mesh)
{
  // Check for transformation - not supported.
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    if (patches[patchI].coupled())
    {
      const coupledPolyPatch& cpp =
        refCast<const coupledPolyPatch>(patches[patchI]);
      if (!cpp.parallel() || cpp.separated())
      {
        FATAL_ERROR_IN
        (
          "extendedCellToCellStencil::extendedCellToCellStencil"
          "(const polyMesh&)"
        )   << "Coupled patches with transformations not supported."
          << endl
          << "Problematic patch " << cpp.name() << exit(FatalError);
      }
    }
  }
}
