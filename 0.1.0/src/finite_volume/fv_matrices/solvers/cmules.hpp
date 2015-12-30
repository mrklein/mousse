// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   cmules.cpp
//   cmules_templates.cpp
#ifndef cmules_hpp_
#define cmules_hpp_
#include "mules.hpp"
#include "euler_ddt_scheme.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "gauss_convection_scheme.hpp"
namespace mousse
{
namespace MULES
{
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void correct
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phi,
  const surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su
);
template<class RhoType, class SpType, class SuType>
void correct
(
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);
void correct
(
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiCorr,
  const scalar psiMax,
  const scalar psiMin
);
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void limiterCorr
(
  scalarField& allLambda,
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phi,
  const surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void limitCorr
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);
}  // namespace MULES
}  // namespace mousse
#ifdef NoRepository
#   include "cmules_templates.cpp"
#endif
#endif
