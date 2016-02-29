#ifndef FINITE_VOLUME_FV_MATRICES_FV_SCALAR_MATRIX_HPP_
#define FINITE_VOLUME_FV_MATRICES_FV_SCALAR_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fv_scalar_matrix.cpp
#include "fv_matrix.hpp"
#include "fv_matrices_fwd.hpp"
namespace mousse
{
// Member Functions 
// Set reference level for a component of the solution
// on a given patch face
template<>
void fvMatrix<scalar>::setComponentReference
(
  const label patchi,
  const label facei,
  const direction,
  const scalar value
);
template<>
autoPtr<fvMatrix<scalar>::fvSolver> fvMatrix<scalar>::solver
(
  const dictionary&
);
template<>
solverPerformance fvMatrix<scalar>::fvSolver::solve
(
  const dictionary&
);
template<>
solverPerformance fvMatrix<scalar>::solveSegregated
(
  const dictionary&
);
template<>
tmp<scalarField> fvMatrix<scalar>::residual() const;
template<>
tmp<volScalarField> fvMatrix<scalar>::H() const;
template<>
tmp<volScalarField> fvMatrix<scalar>::H1() const;
}  // namespace mousse
#endif
