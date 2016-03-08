// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kunz.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace phaseChangeTwoPhaseMixtures
{
DEFINE_TYPE_NAME_AND_DEBUG(Kunz, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(phaseChangeTwoPhaseMixture, Kunz, components);
}
}
// Constructors 
mousse::phaseChangeTwoPhaseMixtures::Kunz::Kunz
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  phaseChangeTwoPhaseMixture{typeName, U, phi},
  UInf_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("UInf")},
  tInf_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("tInf")},
  Cc_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cc")},
  Cv_{phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cv")},
  p0_{"0", pSat().dimensions(), 0.0},
  mcCoeff_{Cc_*rho2()/tInf_},
  mvCoeff_{Cv_*rho2()/(0.5*rho1()*sqr(UInf_)*tInf_)}
{
  correct();
}
// Member Functions 
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::Kunz::mDotAlphal() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  return Pair<tmp<volScalarField>>
  (
    mcCoeff_*sqr(limitedAlpha1)
    *max(p - pSat(), p0_)/max(p - pSat(), 0.01*pSat()),
    mvCoeff_*min(p - pSat(), p0_)
  );
}
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::Kunz::mDotP() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  return Pair<tmp<volScalarField>>
  {
    mcCoeff_*sqr(limitedAlpha1)*(1.0 - limitedAlpha1)
      *pos(p - pSat())/max(p - pSat(), 0.01*pSat()),
    (-mvCoeff_)*limitedAlpha1*neg(p - pSat())
  };
}
void mousse::phaseChangeTwoPhaseMixtures::Kunz::correct()
{}
bool mousse::phaseChangeTwoPhaseMixtures::Kunz::read()
{
  if (phaseChangeTwoPhaseMixture::read())
  {
    phaseChangeTwoPhaseMixtureCoeffs_ = subDict(type() + "Coeffs");
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("UInf") >> UInf_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("tInf") >> tInf_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cc") >> Cc_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cv") >> Cv_;
    mcCoeff_ = Cc_*rho2()/tInf_;
    mvCoeff_ = Cv_*rho2()/(0.5*rho1()*sqr(UInf_)*tInf_);
    return true;
  }
  else
  {
    return false;
  }
}
