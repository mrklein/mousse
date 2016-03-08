// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "merkle.hpp"
#if defined(__GNUC__)
#include "surface_fields.hpp"
#endif
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace phaseChangeTwoPhaseMixtures
{
DEFINE_TYPE_NAME_AND_DEBUG(Merkle, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(phaseChangeTwoPhaseMixture, Merkle, components);
}
}
// Constructors 
mousse::phaseChangeTwoPhaseMixtures::Merkle::Merkle
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
  mcCoeff_{Cc_/(0.5*sqr(UInf_)*tInf_)},
  mvCoeff_{Cv_*rho1()/(0.5*sqr(UInf_)*tInf_*rho2())}
{
  correct();
}
// Member Functions 
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::Merkle::mDotAlphal() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  return Pair<tmp<volScalarField>>
  {
    mcCoeff_*max(p - pSat(), p0_),
    mvCoeff_*min(p - pSat(), p0_)
  };
}
mousse::Pair<mousse::tmp<mousse::volScalarField>>
mousse::phaseChangeTwoPhaseMixtures::Merkle::mDotP() const
{
  const volScalarField& p = alpha1_.db().lookupObject<volScalarField>("p");
  volScalarField limitedAlpha1{min(max(alpha1_, scalar(0)), scalar(1))};
  return Pair<tmp<volScalarField>>
  {
    mcCoeff_*(1.0 - limitedAlpha1)*pos(p - pSat()),
    (-mvCoeff_)*limitedAlpha1*neg(p - pSat())
  };
}
void mousse::phaseChangeTwoPhaseMixtures::Merkle::correct()
{}
bool mousse::phaseChangeTwoPhaseMixtures::Merkle::read()
{
  if (phaseChangeTwoPhaseMixture::read())
  {
    phaseChangeTwoPhaseMixtureCoeffs_ = subDict(type() + "Coeffs");
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("UInf") >> UInf_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("tInf") >> tInf_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cc") >> Cc_;
    phaseChangeTwoPhaseMixtureCoeffs_.lookup("Cv") >> Cv_;
    mcCoeff_ = Cc_/(0.5*sqr(UInf_)*tInf_);
    mvCoeff_ = Cv_*rho1()/(0.5*sqr(UInf_)*tInf_*rho2());
    return true;
  }
  else
  {
    return false;
  }
}
