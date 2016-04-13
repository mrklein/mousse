#ifndef FINITE_VOLUME_FV_MATRICES_SOLVERS_IMULES_HPP_
#define FINITE_VOLUME_FV_MATRICES_SOLVERS_IMULES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mules.hpp"


namespace mousse {
namespace MULES {

template<class RhoType, class SpType, class SuType>
void implicitSolve
(
  const RhoType& rho,
  volScalarField& gamma,
  const surfaceScalarField& phi,
  surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);

void implicitSolve
(
  volScalarField& gamma,
  const surfaceScalarField& phi,
  surfaceScalarField& phiCorr,
  const scalar psiMax,
  const scalar psiMin
);

}  // namespace MULES
}  // namespace mousse

#include "imules.ipp"

#endif
