// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_boundary_mesh.hpp"
#include "fv_mesh.hpp"
#include "primitive_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(fvPatch, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(fvPatch, polyPatch);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, fvPatch, polyPatch);
}

// Constructors
mousse::fvPatch::fvPatch(const polyPatch& p, const fvBoundaryMesh& bm)
:
  polyPatch_(p),
  boundaryMesh_(bm)
{}

// Destructor
mousse::fvPatch::~fvPatch()
{}

// Member Functions
bool mousse::fvPatch::constraintType(const word& pt)
{
  return fvPatchField<scalar>::patchConstructorTablePtr_->found(pt);
}
mousse::wordList mousse::fvPatch::constraintTypes()
{
  wordList cTypes(polyPatchConstructorTablePtr_->size());
  label i = 0;
  for
  (
    polyPatchConstructorTable::iterator cstrIter =
      polyPatchConstructorTablePtr_->begin();
    cstrIter != polyPatchConstructorTablePtr_->end();
    ++cstrIter
  )
  {
    if (constraintType(cstrIter.key()))
    {
      cTypes[i++] = cstrIter.key();
    }
  }
  cTypes.setSize(i);
  return cTypes;
}
const mousse::labelUList& mousse::fvPatch::faceCells() const
{
  return polyPatch_.faceCells();
}
const mousse::vectorField& mousse::fvPatch::Cf() const
{
  return boundaryMesh().mesh().Cf().boundaryField()[index()];
}
mousse::tmp<mousse::vectorField> mousse::fvPatch::Cn() const
{
  tmp<vectorField> tcc(new vectorField(size()));
  vectorField& cc = tcc();
  const labelUList& faceCells = this->faceCells();
  // get reference to global cell centres
  const vectorField& gcc = boundaryMesh().mesh().cellCentres();
  FOR_ALL(faceCells, faceI)
  {
    cc[faceI] = gcc[faceCells[faceI]];
  }
  return tcc;
}
mousse::tmp<mousse::vectorField> mousse::fvPatch::nf() const
{
  return Sf()/magSf();
}
const mousse::vectorField& mousse::fvPatch::Sf() const
{
  return boundaryMesh().mesh().Sf().boundaryField()[index()];
}
const mousse::scalarField& mousse::fvPatch::magSf() const
{
  return boundaryMesh().mesh().magSf().boundaryField()[index()];
}
mousse::tmp<mousse::vectorField> mousse::fvPatch::delta() const
{
  // Use patch-normal delta for all non-coupled BCs
  const vectorField nHat(nf());
  return nHat*(nHat & (Cf() - Cn()));
}
void mousse::fvPatch::makeWeights(scalarField& w) const
{
  w = 1.0;
}
void mousse::fvPatch::initMovePoints()
{}
void mousse::fvPatch::movePoints()
{}
const mousse::scalarField& mousse::fvPatch::deltaCoeffs() const
{
  return boundaryMesh().mesh().deltaCoeffs().boundaryField()[index()];
}
const mousse::scalarField& mousse::fvPatch::weights() const
{
  return boundaryMesh().mesh().weights().boundaryField()[index()];
}
