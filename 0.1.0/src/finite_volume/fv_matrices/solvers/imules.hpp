// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   imules.cpp
//   imules_templates.cpp
#ifndef imules_hpp_
#define imules_hpp_
#include "mules.hpp"
namespace mousse
{
namespace MULES
{
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
#ifdef NoRepository
#   include "imules_templates.cpp"
#endif
#endif
