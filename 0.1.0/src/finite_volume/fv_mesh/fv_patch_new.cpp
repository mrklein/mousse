// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch.hpp"
#include "hash_table.hpp"
#include "cyclic_ami_poly_patch.hpp"
mousse::autoPtr<mousse::fvPatch> mousse::fvPatch::New
(
  const polyPatch& patch,
  const fvBoundaryMesh& bm
)
{
  if (debug)
  {
    Info<< "fvPatch::New(const polyPatch&, const fvBoundaryMesh&) : "
      << "constructing fvPatch"
      << endl;
  }
  polyPatchConstructorTable::iterator cstrIter =
    polyPatchConstructorTablePtr_->find(patch.type());
  if (cstrIter == polyPatchConstructorTablePtr_->end())
  {
    FatalErrorIn("fvPatch::New(const polyPatch&, const fvBoundaryMesh&)")
      << "Unknown fvPatch type " << patch.type() << nl
      << "Valid fvPatch types are :"
      << polyPatchConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<fvPatch>(cstrIter()(patch, bm));
}
