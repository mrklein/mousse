// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pbicg.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(PBiCG, 0);
  lduMatrix::solver::addasymMatrixConstructorToTable<PBiCG>
    addPBiCGAsymMatrixConstructorToTable_;
}
// Constructors 
mousse::PBiCG::PBiCG
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const dictionary& solverControls
)
:
  lduMatrix::solver
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverControls
  )
{}
// Member Functions 
mousse::solverPerformance mousse::PBiCG::solve
(
  scalarField& psi,
  const scalarField& source,
  const direction cmpt
) const
{
  // --- Setup class containing solver performance data
  solverPerformance solverPerf
  (
    lduMatrix::preconditioner::getName(controlDict_) + typeName,
    fieldName_
  );
  label nCells = psi.size();
  scalar* __restrict__ psiPtr = psi.begin();
  scalarField pA(nCells);
  scalar* __restrict__ pAPtr = pA.begin();
  scalarField pT(nCells, 0.0);
  scalar* __restrict__ pTPtr = pT.begin();
  scalarField wA(nCells);
  scalar* __restrict__ wAPtr = wA.begin();
  scalarField wT(nCells);
  scalar* __restrict__ wTPtr = wT.begin();
  scalar wArT = solverPerf.great_;
  scalar wArTold = wArT;
  // --- Calculate A.psi and T.psi
  matrix_.Amul(wA, psi, interfaceBouCoeffs_, interfaces_, cmpt);
  matrix_.Tmul(wT, psi, interfaceIntCoeffs_, interfaces_, cmpt);
  // --- Calculate initial residual and transpose residual fields
  scalarField rA(source - wA);
  scalarField rT(source - wT);
  scalar* __restrict__ rAPtr = rA.begin();
  scalar* __restrict__ rTPtr = rT.begin();
  // --- Calculate normalisation factor
  scalar normFactor = this->normFactor(psi, source, wA, pA);
  if (lduMatrix::debug >= 2)
  {
    Info<< "   Normalisation factor = " << normFactor << endl;
  }
  // --- Calculate normalised residual norm
  solverPerf.initialResidual() =
    gSumMag(rA, matrix().mesh().comm())
   /normFactor;
  solverPerf.finalResidual() = solverPerf.initialResidual();
  // --- Check convergence, solve if not converged
  if
  (
    minIter_ > 0
  || !solverPerf.checkConvergence(tolerance_, relTol_)
  )
  {
    // --- Select and construct the preconditioner
    autoPtr<lduMatrix::preconditioner> preconPtr =
    lduMatrix::preconditioner::New
    (
      *this,
      controlDict_
    );
    // --- Solver iteration
    do
    {
      // --- Store previous wArT
      wArTold = wArT;
      // --- Precondition residuals
      preconPtr->precondition(wA, rA, cmpt);
      preconPtr->preconditionT(wT, rT, cmpt);
      // --- Update search directions:
      wArT = gSumProd(wA, rT, matrix().mesh().comm());
      if (solverPerf.nIterations() == 0)
      {
        for (label cell=0; cell<nCells; cell++)
        {
          pAPtr[cell] = wAPtr[cell];
          pTPtr[cell] = wTPtr[cell];
        }
      }
      else
      {
        scalar beta = wArT/wArTold;
        for (label cell=0; cell<nCells; cell++)
        {
          pAPtr[cell] = wAPtr[cell] + beta*pAPtr[cell];
          pTPtr[cell] = wTPtr[cell] + beta*pTPtr[cell];
        }
      }
      // --- Update preconditioned residuals
      matrix_.Amul(wA, pA, interfaceBouCoeffs_, interfaces_, cmpt);
      matrix_.Tmul(wT, pT, interfaceIntCoeffs_, interfaces_, cmpt);
      scalar wApT = gSumProd(wA, pT, matrix().mesh().comm());
      // --- Test for singularity
      if (solverPerf.checkSingularity(mag(wApT)/normFactor))
      {
        break;
      }
      // --- Update solution and residual:
      scalar alpha = wArT/wApT;
      for (label cell=0; cell<nCells; cell++)
      {
        psiPtr[cell] += alpha*pAPtr[cell];
        rAPtr[cell] -= alpha*wAPtr[cell];
        rTPtr[cell] -= alpha*wTPtr[cell];
      }
      solverPerf.finalResidual() =
        gSumMag(rA, matrix().mesh().comm())
       /normFactor;
    } while
    (
      (
        solverPerf.nIterations()++ < maxIter_
      && !solverPerf.checkConvergence(tolerance_, relTol_)
      )
    || solverPerf.nIterations() < minIter_
    );
  }
  return solverPerf;
}
