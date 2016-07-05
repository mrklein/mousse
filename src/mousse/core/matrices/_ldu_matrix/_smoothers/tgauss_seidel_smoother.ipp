// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tgauss_seidel_smoother.hpp"


// Constructors 
template<class Type, class DType, class LUType>
mousse::TGaussSeidelSmoother<Type, DType, LUType>::TGaussSeidelSmoother
(
  const word& fieldName,
  const LduMatrix<Type, DType, LUType>& matrix
)
:
  LduMatrix<Type, DType, LUType>::smoother
  {
    fieldName,
    matrix
  },
  rD_{matrix.diag().size()}
{
  const label nCells = matrix.diag().size();
  const DType* const __restrict__ diagPtr = matrix.diag().begin();
  DType* __restrict__ rDPtr = rD_.begin();
  for (label cellI=0; cellI<nCells; cellI++)
  {
    rDPtr[cellI] = inv(diagPtr[cellI]);
  }
}


// Member Functions 
template<class Type, class DType, class LUType>
void mousse::TGaussSeidelSmoother<Type, DType, LUType>::smooth
(
  const word& /*fieldName_*/,
  Field<Type>& psi,
  const LduMatrix<Type, DType, LUType>& matrix_,
  const Field<DType>& rD_,
  const label nSweeps
)
{
  Type* __restrict__ psiPtr = psi.begin();
  const label nCells = psi.size();
  Field<Type> bPrime(nCells);
  Type* __restrict__ bPrimePtr = bPrime.begin();
  const DType* const __restrict__ rDPtr = rD_.begin();
  const LUType* const __restrict__ upperPtr =
    matrix_.upper().begin();
  const LUType* const __restrict__ lowerPtr =
    matrix_.lower().begin();
  const label* const __restrict__ uPtr =
    matrix_.lduAddr().upperAddr().begin();
  const label* const __restrict__ ownStartPtr =
    matrix_.lduAddr().ownerStartAddr().begin();
  // Parallel boundary initialisation.  The parallel boundary is treated
  // as an effective jacobi interface in the boundary.
  // Note: there is a change of sign in the coupled
  // interface update to add the contibution to the r.h.s.
  FieldField<Field, LUType> mBouCoeffs(matrix_.interfacesUpper().size());
  FOR_ALL(mBouCoeffs, patchi) {
    if (matrix_.interfaces().set(patchi)) {
      mBouCoeffs.set(patchi, -matrix_.interfacesUpper()[patchi]);
    }
  }
  for (label sweep=0; sweep<nSweeps; sweep++) {
    bPrime = matrix_.source();
    matrix_.initMatrixInterfaces
    (
      mBouCoeffs,
      psi,
      bPrime
    );
    matrix_.updateMatrixInterfaces
    (
      mBouCoeffs,
      psi,
      bPrime
    );
    Type curPsi;
    label fStart;
    label fEnd = ownStartPtr[0];
    for (label cellI=0; cellI<nCells; cellI++) {
      // Start and end of this row
      fStart = fEnd;
      fEnd = ownStartPtr[cellI + 1];
      // Get the accumulated neighbour side
      curPsi = bPrimePtr[cellI];
      // Accumulate the owner product side
      for (label curFace=fStart; curFace<fEnd; curFace++) {
        curPsi -= dot(upperPtr[curFace], psiPtr[uPtr[curFace]]);
      }
      // Finish current psi
      curPsi = dot(rDPtr[cellI], curPsi);
      // Distribute the neighbour side using current psi
      for (label curFace=fStart; curFace<fEnd; curFace++) {
        bPrimePtr[uPtr[curFace]] -= dot(lowerPtr[curFace], curPsi);
      }
      psiPtr[cellI] = curPsi;
    }
  }
}


template<class Type, class DType, class LUType>
void mousse::TGaussSeidelSmoother<Type, DType, LUType>::smooth
(
  Field<Type>& psi,
  const label nSweeps
) const
{
  smooth(this->fieldName_, psi, this->matrix_, rD_, nSweeps);
}
