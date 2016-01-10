// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dic_smoother.hpp"
#include "dic_preconditioner.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(DICSmoother, 0);
  lduMatrix::smoother::addsymMatrixConstructorToTable<DICSmoother>
    addDICSmootherSymMatrixConstructorToTable_;
}
// Constructors 
mousse::DICSmoother::DICSmoother
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces
)
:
  lduMatrix::smoother
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  ),
  rD_(matrix_.diag())
{
  DICPreconditioner::calcReciprocalD(rD_, matrix_);
}
// Member Functions 
void mousse::DICSmoother::smooth
(
  scalarField& psi,
  const scalarField& source,
  const direction cmpt,
  const label nSweeps
) const
{
  const scalar* const __restrict__ rDPtr = rD_.begin();
  const scalar* const __restrict__ upperPtr = matrix_.upper().begin();
  const label* const __restrict__ uPtr =
    matrix_.lduAddr().upperAddr().begin();
  const label* const __restrict__ lPtr =
    matrix_.lduAddr().lowerAddr().begin();
  // Temporary storage for the residual
  scalarField rA(rD_.size());
  scalar* __restrict__ rAPtr = rA.begin();
  for (label sweep=0; sweep<nSweeps; sweep++)
  {
    matrix_.residual
    (
      rA,
      psi,
      source,
      interfaceBouCoeffs_,
      interfaces_,
      cmpt
    );
    rA *= rD_;
    label nFaces = matrix_.upper().size();
    for (label facei=0; facei<nFaces; facei++)
    {
      label u = uPtr[facei];
      rAPtr[u] -= rDPtr[u]*upperPtr[facei]*rAPtr[lPtr[facei]];
    }
    label nFacesM1 = nFaces - 1;
    for (label facei=nFacesM1; facei>=0; facei--)
    {
      label l = lPtr[facei];
      rAPtr[l] -= rDPtr[l]*upperPtr[facei]*rAPtr[uPtr[facei]];
    }
    psi += rA;
  }
}
