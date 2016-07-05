// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dic_preconditioner.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(DICPreconditioner, 0);
lduMatrix::preconditioner::
  addsymMatrixConstructorToTable<DICPreconditioner>
  addDICPreconditionerSymMatrixConstructorToTable_;

}

// Constructors
mousse::DICPreconditioner::DICPreconditioner
(
  const lduMatrix::solver& sol,
  const dictionary&
)
:
  lduMatrix::preconditioner{sol},
  rD_{sol.matrix().diag()}
{
  calcReciprocalD(rD_, sol.matrix());
}

// Member Functions
void mousse::DICPreconditioner::calcReciprocalD
(
  scalarField& rD,
  const lduMatrix& matrix
)
{
  scalar* __restrict__ rDPtr = rD.begin();
  const label* const __restrict__ uPtr = matrix.lduAddr().upperAddr().begin();
  const label* const __restrict__ lPtr = matrix.lduAddr().lowerAddr().begin();
  const scalar* const __restrict__ upperPtr = matrix.upper().begin();
  // Calculate the DIC diagonal
  const label nFaces = matrix.upper().size();
  for (label face=0; face<nFaces; face++) {
    rDPtr[uPtr[face]] -= upperPtr[face]*upperPtr[face]/rDPtr[lPtr[face]];
  }
  // Calculate the reciprocal of the preconditioned diagonal
  const label nCells = rD.size();
  for (label cell=0; cell<nCells; cell++) {
    rDPtr[cell] = 1.0/rDPtr[cell];
  }
}


void mousse::DICPreconditioner::precondition
(
  scalarField& wA,
  const scalarField& rA,
  const direction
) const
{
  scalar* __restrict__ wAPtr = wA.begin();
  const scalar* __restrict__ rAPtr = rA.begin();
  const scalar* __restrict__ rDPtr = rD_.begin();
  const label* const __restrict__ uPtr =
    solver_.matrix().lduAddr().upperAddr().begin();
  const label* const __restrict__ lPtr =
    solver_.matrix().lduAddr().lowerAddr().begin();
  const scalar* const __restrict__ upperPtr =
    solver_.matrix().upper().begin();
  label nCells = wA.size();
  label nFaces = solver_.matrix().upper().size();
  label nFacesM1 = nFaces - 1;
  for (label cell=0; cell<nCells; cell++) {
    wAPtr[cell] = rDPtr[cell]*rAPtr[cell];
  }
  for (label face=0; face<nFaces; face++) {
    wAPtr[uPtr[face]] -= rDPtr[uPtr[face]]*upperPtr[face]*wAPtr[lPtr[face]];
  }
  for (label face=nFacesM1; face>=0; face--) {
    wAPtr[lPtr[face]] -= rDPtr[lPtr[face]]*upperPtr[face]*wAPtr[uPtr[face]];
  }
}
