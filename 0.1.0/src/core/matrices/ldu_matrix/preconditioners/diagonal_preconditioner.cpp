// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diagonal_preconditioner.hpp"
// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(diagonalPreconditioner, 0);
lduMatrix::preconditioner::
  addsymMatrixConstructorToTable<diagonalPreconditioner>
  adddiagonalPreconditionerSymMatrixConstructorToTable_;
lduMatrix::preconditioner::
  addasymMatrixConstructorToTable<diagonalPreconditioner>
  adddiagonalPreconditionerAsymMatrixConstructorToTable_;

}

// Constructors
mousse::diagonalPreconditioner::diagonalPreconditioner
(
  const lduMatrix::solver& sol,
  const dictionary&
)
:
  lduMatrix::preconditioner(sol),
  rD(sol.matrix().diag().size())
{
  scalar* __restrict__ rDPtr = rD.begin();
  const scalar* __restrict__ DPtr = solver_.matrix().diag().begin();
  label nCells = rD.size();
  // Generate reciprocal diagonal
  for (label cell=0; cell<nCells; cell++)
  {
    rDPtr[cell] = 1.0/DPtr[cell];
  }
}

// Member Functions
void mousse::diagonalPreconditioner::precondition
(
  scalarField& wA,
  const scalarField& rA,
  const direction
) const
{
  scalar* __restrict__ wAPtr = wA.begin();
  const scalar* __restrict__ rAPtr = rA.begin();
  const scalar* __restrict__ rDPtr = rD.begin();
  label nCells = wA.size();
  for (label cell=0; cell<nCells; cell++)
  {
    wAPtr[cell] = rDPtr[cell]*rAPtr[cell];
  }
}
