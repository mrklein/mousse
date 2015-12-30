// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_scalar_matrix.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Member Functions 
template<>
void mousse::fvMatrix<mousse::scalar>::setComponentReference
(
  const label patchi,
  const label facei,
  const direction,
  const scalar value
)
{
  if (psi_.needReference())
  {
    if (Pstream::master())
    {
      internalCoeffs_[patchi][facei] +=
        diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]];
      boundaryCoeffs_[patchi][facei] +=
        diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]]
       *value;
    }
  }
}
template<>
mousse::autoPtr<mousse::fvMatrix<mousse::scalar>::fvSolver>
mousse::fvMatrix<mousse::scalar>::solver
(
  const dictionary& solverControls
)
{
  if (debug)
  {
    Info.masterStream(this->mesh().comm())
      << "fvMatrix<scalar>::solver(const dictionary& solverControls) : "
       "solver for fvMatrix<scalar>"
      << endl;
  }
  scalarField saveDiag(diag());
  addBoundaryDiag(diag(), 0);
  autoPtr<fvMatrix<scalar>::fvSolver> solverPtr
  (
    new fvMatrix<scalar>::fvSolver
    (
      *this,
      lduMatrix::solver::New
      (
        psi_.name(),
        *this,
        boundaryCoeffs_,
        internalCoeffs_,
        psi_.boundaryField().scalarInterfaces(),
        solverControls
      )
    )
  );
  diag() = saveDiag;
  return solverPtr;
}
template<>
mousse::solverPerformance mousse::fvMatrix<mousse::scalar>::fvSolver::solve
(
  const dictionary& solverControls
)
{
  GeometricField<scalar, fvPatchField, volMesh>& psi =
    const_cast<GeometricField<scalar, fvPatchField, volMesh>&>
    (fvMat_.psi());
  scalarField saveDiag(fvMat_.diag());
  fvMat_.addBoundaryDiag(fvMat_.diag(), 0);
  scalarField totalSource(fvMat_.source());
  fvMat_.addBoundarySource(totalSource, false);
  // assign new solver controls
  solver_->read(solverControls);
  solverPerformance solverPerf = solver_->solve
  (
    psi.internalField(),
    totalSource
  );
  if (solverPerformance::debug)
  {
    solverPerf.print(Info.masterStream(fvMat_.mesh().comm()));
  }
  fvMat_.diag() = saveDiag;
  psi.correctBoundaryConditions();
  psi.mesh().setSolverPerformance(psi.name(), solverPerf);
  return solverPerf;
}
template<>
mousse::solverPerformance mousse::fvMatrix<mousse::scalar>::solveSegregated
(
  const dictionary& solverControls
)
{
  if (debug)
  {
    Info.masterStream(this->mesh().comm())
      << "fvMatrix<scalar>::solveSegregated"
       "(const dictionary& solverControls) : "
       "solving fvMatrix<scalar>"
      << endl;
  }
  GeometricField<scalar, fvPatchField, volMesh>& psi =
   const_cast<GeometricField<scalar, fvPatchField, volMesh>&>(psi_);
  scalarField saveDiag(diag());
  addBoundaryDiag(diag(), 0);
  scalarField totalSource(source_);
  addBoundarySource(totalSource, false);
  // Solver call
  solverPerformance solverPerf = lduMatrix::solver::New
  (
    psi.name(),
    *this,
    boundaryCoeffs_,
    internalCoeffs_,
    psi.boundaryField().scalarInterfaces(),
    solverControls
  )->solve(psi.internalField(), totalSource);
  if (solverPerformance::debug)
  {
    solverPerf.print(Info.masterStream(mesh().comm()));
  }
  diag() = saveDiag;
  psi.correctBoundaryConditions();
  psi.mesh().setSolverPerformance(psi.name(), solverPerf);
  return solverPerf;
}
template<>
mousse::tmp<mousse::scalarField> mousse::fvMatrix<mousse::scalar>::residual() const
{
  scalarField boundaryDiag(psi_.size(), 0.0);
  addBoundaryDiag(boundaryDiag, 0);
  tmp<scalarField> tres
  (
    lduMatrix::residual
    (
      psi_.internalField(),
      source_ - boundaryDiag*psi_.internalField(),
      boundaryCoeffs_,
      psi_.boundaryField().scalarInterfaces(),
      0
    )
  );
  addBoundarySource(tres());
  return tres;
}
template<>
mousse::tmp<mousse::volScalarField> mousse::fvMatrix<mousse::scalar>::H() const
{
  tmp<volScalarField> tHphi
  (
    new volScalarField
    (
      IOobject
      (
        "H("+psi_.name()+')',
        psi_.instance(),
        psi_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      psi_.mesh(),
      dimensions_/dimVol,
      zeroGradientFvPatchScalarField::typeName
    )
  );
  volScalarField& Hphi = tHphi();
  Hphi.internalField() = (lduMatrix::H(psi_.internalField()) + source_);
  addBoundarySource(Hphi.internalField());
  Hphi.internalField() /= psi_.mesh().V();
  Hphi.correctBoundaryConditions();
  return tHphi;
}
template<>
mousse::tmp<mousse::volScalarField> mousse::fvMatrix<mousse::scalar>::H1() const
{
  tmp<volScalarField> tH1
  (
    new volScalarField
    (
      IOobject
      (
        "H(1)",
        psi_.instance(),
        psi_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      psi_.mesh(),
      dimensions_/(dimVol*psi_.dimensions()),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  volScalarField& H1_ = tH1();
  H1_.internalField() = lduMatrix::H1();
  //addBoundarySource(Hphi.internalField());
  H1_.internalField() /= psi_.mesh().V();
  H1_.correctBoundaryConditions();
  return tH1;
}
