// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "non_blocking_gauss_seidel_smoother.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(nonBlockingGaussSeidelSmoother, 0);
  lduMatrix::smoother::
  addsymMatrixConstructorToTable<nonBlockingGaussSeidelSmoother>
    addnonBlockingGaussSeidelSmootherSymMatrixConstructorToTable_;
  lduMatrix::smoother::
  addasymMatrixConstructorToTable<nonBlockingGaussSeidelSmoother>
    addnonBlockingGaussSeidelSmootherAsymMatrixConstructorToTable_;
}
// Constructors 
mousse::nonBlockingGaussSeidelSmoother::nonBlockingGaussSeidelSmoother
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
  )
{
  // Check that all interface addressing is sorted to be after the
  // non-interface addressing.
  const label nCells = matrix.diag().size();
  blockStart_ = nCells;
  labelList startCellI(interfaceBouCoeffs.size(), -1);
  FOR_ALL(interfaces, patchi)
  {
    if (interfaces.set(patchi))
    {
      const labelUList& faceCells = matrix_.lduAddr().patchAddr(patchi);
      blockStart_ = min(blockStart_, min(faceCells));
    }
  }
  if (debug)
  {
    Pout<< "nonBlockingGaussSeidelSmoother :"
      << " Starting block on cell " << blockStart_
      << " out of " << nCells << endl;
  }
}
// Member Functions 
void mousse::nonBlockingGaussSeidelSmoother::smooth
(
  const word& /*fieldName_*/,
  scalarField& psi,
  const lduMatrix& matrix_,
  const label blockStart,
  const scalarField& source,
  const FieldField<Field, scalar>& interfaceBouCoeffs_,
  const lduInterfaceFieldPtrsList& interfaces_,
  const direction cmpt,
  const label nSweeps
)
{
  scalar* __restrict__ psiPtr = psi.begin();
  const label nCells = psi.size();
  scalarField bPrime(nCells);
  scalar* __restrict__ bPrimePtr = bPrime.begin();
  const scalar* const __restrict__ diagPtr = matrix_.diag().begin();
  const scalar* const __restrict__ upperPtr =
    matrix_.upper().begin();
  const scalar* const __restrict__ lowerPtr =
    matrix_.lower().begin();
  const label* const __restrict__ uPtr =
    matrix_.lduAddr().upperAddr().begin();
  const label* const __restrict__ ownStartPtr =
    matrix_.lduAddr().ownerStartAddr().begin();
  // Parallel boundary initialisation.  The parallel boundary is treated
  // as an effective jacobi interface in the boundary.
  // Note: there is a change of sign in the coupled
  // interface update.  The reason for this is that the
  // internal coefficients are all located at the l.h.s. of
  // the matrix whereas the "implicit" coefficients on the
  // coupled boundaries are all created as if the
  // coefficient contribution is of a source-kind (i.e. they
  // have a sign as if they are on the r.h.s. of the matrix.
  // To compensate for this, it is necessary to turn the
  // sign of the contribution.
  FieldField<Field, scalar>& mBouCoeffs =
    const_cast<FieldField<Field, scalar>&>
    (
      interfaceBouCoeffs_
    );
  FOR_ALL(mBouCoeffs, patchi)
  {
    if (interfaces_.set(patchi))
    {
      mBouCoeffs[patchi].negate();
    }
  }
  for (label sweep=0; sweep<nSweeps; sweep++)
  {
    bPrime = source;
    matrix_.initMatrixInterfaces
    (
      mBouCoeffs,
      interfaces_,
      psi,
      bPrime,
      cmpt
    );
    scalar curPsi;
    label fStart;
    label fEnd = ownStartPtr[0];
    for (label cellI=0; cellI<blockStart; cellI++)
    {
      // Start and end of this row
      fStart = fEnd;
      fEnd = ownStartPtr[cellI + 1];
      // Get the accumulated neighbour side
      curPsi = bPrimePtr[cellI];
      // Accumulate the owner product side
      for (label curFace=fStart; curFace<fEnd; curFace++)
      {
        curPsi -= upperPtr[curFace]*psiPtr[uPtr[curFace]];
      }
      // Finish current psi
      curPsi /= diagPtr[cellI];
      // Distribute the neighbour side using current psi
      for (label curFace=fStart; curFace<fEnd; curFace++)
      {
        bPrimePtr[uPtr[curFace]] -= lowerPtr[curFace]*curPsi;
      }
      psiPtr[cellI] = curPsi;
    }
    matrix_.updateMatrixInterfaces
    (
      mBouCoeffs,
      interfaces_,
      psi,
      bPrime,
      cmpt
    );
    // Update rest of the cells
    for (label cellI=blockStart; cellI < nCells; cellI++)
    {
      // Start and end of this row
      fStart = fEnd;
      fEnd = ownStartPtr[cellI + 1];
      // Get the accumulated neighbour side
      curPsi = bPrimePtr[cellI];
      // Accumulate the owner product side
      for (label curFace=fStart; curFace<fEnd; curFace++)
      {
        curPsi -= upperPtr[curFace]*psiPtr[uPtr[curFace]];
      }
      // Finish current psi
      curPsi /= diagPtr[cellI];
      // Distribute the neighbour side using current psi
      for (label curFace=fStart; curFace<fEnd; curFace++)
      {
        bPrimePtr[uPtr[curFace]] -= lowerPtr[curFace]*curPsi;
      }
      psiPtr[cellI] = curPsi;
    }
  }
  // Restore interfaceBouCoeffs_
  FOR_ALL(mBouCoeffs, patchi)
  {
    if (interfaces_.set(patchi))
    {
      mBouCoeffs[patchi].negate();
    }
  }
}
void mousse::nonBlockingGaussSeidelSmoother::smooth
(
  scalarField& psi,
  const scalarField& source,
  const direction cmpt,
  const label nSweeps
) const
{
  smooth
  (
    fieldName_,
    psi,
    matrix_,
    blockStart_,
    source,
    interfaceBouCoeffs_,
    interfaces_,
    cmpt,
    nSweeps
  );
}
