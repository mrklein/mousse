// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_preconditioner.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(noPreconditioner, 0);
lduMatrix::preconditioner::
  addsymMatrixConstructorToTable<noPreconditioner>
  addnoPreconditionerSymMatrixConstructorToTable_;
lduMatrix::preconditioner::
  addasymMatrixConstructorToTable<noPreconditioner>
  addnoPreconditionerAsymMatrixConstructorToTable_;

}

// Constructors 
mousse::noPreconditioner::noPreconditioner
(
  const lduMatrix::solver& sol,
  const dictionary&
)
:
  lduMatrix::preconditioner(sol)
{}
// Member Functions 
void mousse::noPreconditioner::precondition
(
  scalarField& wA,
  const scalarField& rA,
  const direction
) const
{
  scalar* __restrict__ wAPtr = wA.begin();
  const scalar* __restrict__ rAPtr = rA.begin();
  label nCells = wA.size();
  for (label cell=0; cell<nCells; cell++)
  {
    wAPtr[cell] = rAPtr[cell];
  }
}
