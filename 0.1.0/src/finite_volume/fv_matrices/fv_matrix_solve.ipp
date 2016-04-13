// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_matrix.hpp"
#include "diag_tensor_field.hpp"


// Member Functions 
template<class Type>
void mousse::fvMatrix<Type>::setComponentReference
(
  const label patchi,
  const label facei,
  const direction cmpt,
  const scalar value
)
{
  if (psi_.needReference()) {
    if (Pstream::master()) {
      internalCoeffs_[patchi][facei].component(cmpt) +=
        diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]];
      boundaryCoeffs_[patchi][facei].component(cmpt) +=
        diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]]
       *value;
    }
  }
}


template<class Type>
mousse::solverPerformance mousse::fvMatrix<Type>::solve
(
  const dictionary& solverControls
)
{
  if (debug) {
    Info.masterStream(this->mesh().comm())
      << "fvMatrix<Type>::solve(const dictionary& solverControls) : "
       "solving fvMatrix<Type>"
      << endl;
  }
  label maxIter = -1;
  if (solverControls.readIfPresent("maxIter", maxIter)) {
    if (maxIter == 0) {
      return solverPerformance();
    }
  }
  word type{solverControls.lookupOrDefault<word>("type", "segregated")};
  if (type == "segregated") {
    return solveSegregated(solverControls);
  } else if (type == "coupled") {
    return solveCoupled(solverControls);
  } else {
    FATAL_IO_ERROR_IN
    (
      "fvMatrix<Type>::solve(const dictionary& solverControls)",
      solverControls
    )
    << "Unknown type " << type
    << "; currently supported solver types are segregated and coupled"
    << exit(FatalIOError);
    return solverPerformance();
  }
}


template<class Type>
mousse::solverPerformance mousse::fvMatrix<Type>::solveSegregated
(
  const dictionary& solverControls
)
{
  if (debug) {
    Info.masterStream(this->mesh().comm())
      << "fvMatrix<Type>::solveSegregated"
       "(const dictionary& solverControls) : "
       "solving fvMatrix<Type>"
      << endl;
  }
  GeometricField<Type, fvPatchField, volMesh>& psi =
   const_cast<GeometricField<Type, fvPatchField, volMesh>&>(psi_);
  solverPerformance solverPerfVec
  {
    "fvMatrix<Type>::solveSegregated",
    psi.name()
  };
  scalarField saveDiag{diag()};
  Field<Type> source{source_};
  // At this point include the boundary source from the coupled boundaries.
  // This is corrected for the implict part by updateMatrixInterfaces within
  // the component loop.
  addBoundarySource(source);
  typename Type::labelType validComponents
  {
    pow
    (
      psi.mesh().solutionD(),
      pTraits<typename powProduct<Vector<label>, Type::rank>::type>::zero
    )
  };
  for (direction cmpt=0; cmpt<Type::nComponents; cmpt++) {
    if (validComponents[cmpt] == -1)
      continue;
    // copy field and source
    scalarField psiCmpt{psi.internalField().component(cmpt)};
    addBoundaryDiag(diag(), cmpt);
    scalarField sourceCmpt{source.component(cmpt)};
    FieldField<Field, scalar> bouCoeffsCmpt
    {
      boundaryCoeffs_.component(cmpt)
    };
    FieldField<Field, scalar> intCoeffsCmpt
    {
      internalCoeffs_.component(cmpt)
    };
    lduInterfaceFieldPtrsList interfaces =
      psi.boundaryField().scalarInterfaces();
    // Use the initMatrixInterfaces and updateMatrixInterfaces to correct
    // bouCoeffsCmpt for the explicit part of the coupled boundary
    // conditions
    initMatrixInterfaces
    (
      bouCoeffsCmpt,
      interfaces,
      psiCmpt,
      sourceCmpt,
      cmpt
    );
    updateMatrixInterfaces
    (
      bouCoeffsCmpt,
      interfaces,
      psiCmpt,
      sourceCmpt,
      cmpt
    );
    solverPerformance solverPerf;
    // Solver call
    solverPerf = lduMatrix::solver::New
    (
      psi.name() + pTraits<Type>::componentNames[cmpt],
      *this,
      bouCoeffsCmpt,
      intCoeffsCmpt,
      interfaces,
      solverControls
    )->solve(psiCmpt, sourceCmpt, cmpt);
    if (solverPerformance::debug) {
      solverPerf.print(Info.masterStream(this->mesh().comm()));
    }
    solverPerfVec = max(solverPerfVec, solverPerf);
    solverPerfVec.solverName() = solverPerf.solverName();
    psi.internalField().replace(cmpt, psiCmpt);
    diag() = saveDiag;
  }
  psi.correctBoundaryConditions();
  psi.mesh().setSolverPerformance(psi.name(), solverPerfVec);
  return solverPerfVec;
}


template<class Type>
mousse::solverPerformance mousse::fvMatrix<Type>::solveCoupled
(
  const dictionary& solverControls
)
{
  if (debug) {
    Info.masterStream(this->mesh().comm())
      << "fvMatrix<Type>::solveCoupled"
       "(const dictionary& solverControls) : "
       "solving fvMatrix<Type>"
      << endl;
  }
  GeometricField<Type, fvPatchField, volMesh>& psi =
    const_cast<GeometricField<Type, fvPatchField, volMesh>&>(psi_);
  LduMatrix<Type, scalar, scalar> coupledMatrix{psi.mesh()};
  coupledMatrix.diag() = diag();
  coupledMatrix.upper() = upper();
  coupledMatrix.lower() = lower();
  coupledMatrix.source() = source();
  addBoundaryDiag(coupledMatrix.diag(), 0);
  addBoundarySource(coupledMatrix.source(), false);
  coupledMatrix.interfaces() = psi.boundaryField().interfaces();
  coupledMatrix.interfacesUpper() = boundaryCoeffs().component(0);
  coupledMatrix.interfacesLower() = internalCoeffs().component(0);
  autoPtr<typename LduMatrix<Type, scalar, scalar>::solver>
    coupledMatrixSolver
    {
      LduMatrix<Type, scalar, scalar>::solver::New
      (
        psi.name(),
        coupledMatrix,
        solverControls
      )
    };
  SolverPerformance<Type> solverPerf{coupledMatrixSolver->solve(psi)};
  if (SolverPerformance<Type>::debug) {
    solverPerf.print(Info.masterStream(this->mesh().comm()));
  }
  psi.correctBoundaryConditions();
  // psi.mesh().setSolverPerformance(psi.name(), solverPerf);
  return solverPerformance();
}


template<class Type>
mousse::autoPtr<typename mousse::fvMatrix<Type>::fvSolver>
mousse::fvMatrix<Type>::solver()
{
  return solver
  (
    psi_.mesh().solverDict
    (
      psi_.select
      (
        psi_
          .mesh()
          .data::template lookupOrDefault<bool>("finalIteration", false)
      )
    )
  );
}


template<class Type>
mousse::solverPerformance mousse::fvMatrix<Type>::fvSolver::solve()
{
  return solve
  (
    fvMat_.psi_.mesh().solverDict
    (
      fvMat_.psi_.select
      (
        fvMat_
          .psi_
          .mesh()
          .data::template lookupOrDefault<bool>("finalIteration", false)
      )
    )
  );
}


template<class Type>
mousse::solverPerformance mousse::fvMatrix<Type>::solve()
{
  return solve
  (
    psi_.mesh().solverDict
    (
      psi_.select
      (
        psi_
          .mesh()
          .data::template lookupOrDefault<bool>("finalIteration", false)
      )
    )
  );
}


template<class Type>
mousse::tmp<mousse::Field<Type>> mousse::fvMatrix<Type>::residual() const
{
  tmp<Field<Type>> tres{new Field<Type>{source_}};
  Field<Type>& res = tres();
  addBoundarySource(res);
  // Loop over field components
  for (direction cmpt=0; cmpt<Type::nComponents; cmpt++) {
    scalarField psiCmpt{psi_.internalField().component(cmpt)};
    scalarField boundaryDiagCmpt{psi_.size(), 0.0};
    addBoundaryDiag(boundaryDiagCmpt, cmpt);
    FieldField<Field, scalar> bouCoeffsCmpt
    {
      boundaryCoeffs_.component(cmpt)
    };
    res.replace
    (
      cmpt,
      lduMatrix::residual
      (
        psiCmpt,
        res.component(cmpt) - boundaryDiagCmpt*psiCmpt,
        bouCoeffsCmpt,
        psi_.boundaryField().scalarInterfaces(),
        cmpt
      )
    );
  }
  return tres;
}

