#ifndef FINITE_VOLUME_FV_MATRICES_SOLVERS_MULES_HPP_
#define FINITE_VOLUME_FV_MATRICES_SOLVERS_MULES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   mules.cpp
//   mules_templates.cpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "primitive_fields_fwd.hpp"
#include "geometric_one_field.hpp"
#include "zero.hpp"
#include "zero_field.hpp"
#include "uptr_list.hpp"
namespace mousse
{
namespace MULES
{
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void explicitSolve
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su
);
template<class RhoType, class SpType, class SuType>
void explicitSolve
(
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su
);
template<class RhoType, class SpType, class SuType>
void explicitSolve
(
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phiBD,
  surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);
void explicitSolve
(
  volScalarField& psi,
  const surfaceScalarField& phiBD,
  surfaceScalarField& phiPsi,
  const scalar psiMax,
  const scalar psiMin
);
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void limiter
(
  scalarField& allLambda,
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phiBD,
  const surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
);
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void limit
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin,
  const bool returnCorr
);
void limitSum(UPtrList<scalarField>& phiPsiCorrs);
template<class SurfaceScalarFieldList>
void limitSum(SurfaceScalarFieldList& phiPsiCorrs);
}  // namespace MULES
}  // namespace mousse
#ifdef NoRepository
#   include "mules_templates.cpp"
#endif
#endif
