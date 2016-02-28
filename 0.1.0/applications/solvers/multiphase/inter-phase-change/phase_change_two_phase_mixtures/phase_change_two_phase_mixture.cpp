// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_change_two_phase_mixture.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(phaseChangeTwoPhaseMixture, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(phaseChangeTwoPhaseMixture, components);
}
// Constructors 
mousse::phaseChangeTwoPhaseMixture::phaseChangeTwoPhaseMixture
(
  const word& type,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  incompressibleTwoPhaseMixture{U, phi},
  phaseChangeTwoPhaseMixtureCoeffs_{subDict(type + "Coeffs")},
  pSat_{"pSat", dimPressure, lookup("pSat")}
{}
// Member Functions 
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixture::vDotAlphal() const
{
  volScalarField alphalCoeff{1.0/rho1() - alpha1_*(1.0/rho1() - 1.0/rho2())};
  Pair<tmp<volScalarField>> mDotAlphal = this->mDotAlphal();
  return Pair<tmp<volScalarField>>
  {
    alphalCoeff*mDotAlphal[0],
    alphalCoeff*mDotAlphal[1]
  };
}
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixture::vDotP() const
{
  dimensionedScalar pCoeff{1.0/rho1() - 1.0/rho2()};
  Pair<tmp<volScalarField>> mDotP = this->mDotP();
  return Pair<tmp<volScalarField>>{pCoeff*mDotP[0], pCoeff*mDotP[1]};
}
bool mousse::phaseChangeTwoPhaseMixture::read()
{
  if (incompressibleTwoPhaseMixture::read())
  {
    phaseChangeTwoPhaseMixtureCoeffs_ = subDict(type() + "Coeffs");
    lookup("pSat") >> pSat_;
    return true;
  }
  else
  {
    return false;
  }
}
