// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "schnerr_sauer.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace phaseChangeTwoPhaseMixtures
{
DEFINE_TYPE_NAME_AND_DEBUG(SchnerrSauer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  phaseChangeTwoPhaseMixture,
  SchnerrSauer,
  components
);
}
}
// Constructors 
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::SchnerrSauer
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  phaseChangeTwoPhaseMixture{typeName, U, phi},
  n_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("n")},
  dNuc_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("dNuc")},
  Cc_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cc")},
  Cv_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cv")},
  p0_{"0", pSat().dimensions(), 0.0}
{
  correct();
}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::rRb
(
  const volScalarField& limitedAlpha1
) const
{
  return pow
  (
    ((4*constant::mathematical::pi*n_)/3)
    *limitedAlpha1/(1.0 + alphaNuc() - limitedAlpha1), 1.0/3.0
  );
}
mousse::dimensionedScalar
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::alphaNuc() const
{
  dimensionedScalar Vnuc = n_*constant::mathematical::pi*pow3(dNuc_)/6;
  return Vnuc/(1 + Vnuc);
}
mousse::tmp<mousse::volScalarField>
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::pCoeff
(
  const volScalarField& p
) const
{
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  volScalarField rho
  {
    limitedAlpha1*rho1() + (scalar(1) - limitedAlpha1)*rho2()
  };
  return
    (3*rho1()*rho2())*sqrt(2/(3*rho1()))
    *rRb(limitedAlpha1)/(rho*sqrt(mag(p - pSat()) + 0.01*pSat()));
}
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::mDotAlphal() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  volScalarField pCoeff{this->pCoeff(p)};
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  return Pair<tmp<volScalarField>>
  {
    Cc_*limitedAlpha1*pCoeff*max(p - pSat(), p0_),
    Cv_*(1.0 + alphaNuc() - limitedAlpha1)*pCoeff*min(p - pSat(), p0_)
  };
}
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::mDotP() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  volScalarField pCoeff{this->pCoeff(p)};
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  volScalarField apCoeff{limitedAlpha1*pCoeff};
  return Pair<tmp<volScalarField>>
  {
    Cc_*(1.0 - limitedAlpha1)*pos(p - pSat())*apCoeff,
    (-Cv_)*(1.0 + alphaNuc() - limitedAlpha1)*neg(p - pSat())*apCoeff
  };
}
void mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::correct()
{}
bool mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer::read()
{
  if (phaseChangeTwoPhaseMixture::read())
  {
    phaseChangeTwoPhaseMixtureCoeffs_ = subDict(type() + "Coeffs");
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("n") >> n_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("dNuc") >> dNuc_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cc") >> Cc_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cv") >> Cv_;
    return true;
  }
  else
  {
    return false;
  }
}
