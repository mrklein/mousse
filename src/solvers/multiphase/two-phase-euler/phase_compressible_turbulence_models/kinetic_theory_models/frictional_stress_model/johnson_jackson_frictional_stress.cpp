// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "johnson_jackson_frictional_stress.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mathematical_constants.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace frictionalStressModels {
DEFINE_TYPE_NAME_AND_DEBUG(JohnsonJackson, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  frictionalStressModel,
  JohnsonJackson,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::
JohnsonJackson
(
  const dictionary& dict
)
:
  frictionalStressModel{dict},
  coeffDict_{dict.subDict(typeName + "Coeffs")},
  Fr_{"Fr", {1, -1, -2, 0, 0}, coeffDict_},
  eta_{"eta", dimless, coeffDict_},
  p_{"p", dimless, coeffDict_},
  phi_{"phi", dimless, coeffDict_},
  alphaDeltaMin_{"alphaDeltaMin", dimless, coeffDict_}
{
  phi_ *= constant::mathematical::pi/180.0;
}
// Destructor 
mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::
~JohnsonJackson()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::
frictionalPressure
(
  const volScalarField& alpha1,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& alphaMax
) const
{
  return
    Fr_*pow(max(alpha1 - alphaMinFriction, scalar(0)), eta_)
   /pow(max(alphaMax - alpha1, alphaDeltaMin_), p_);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::
frictionalPressurePrime
(
  const volScalarField& alpha1,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& alphaMax
) const
{
  return Fr_*
  (
    eta_*pow(max(alpha1 - alphaMinFriction, scalar(0)), eta_ - 1.0)
   *(alphaMax-alpha1)
   + p_*pow(max(alpha1 - alphaMinFriction, scalar(0)), eta_)
  )/pow(max(alphaMax - alpha1, alphaDeltaMin_), p_ + 1.0);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::nu
(
  const volScalarField& /*alpha1*/,
  const dimensionedScalar& /*alphaMinFriction*/,
  const dimensionedScalar& /*alphaMax*/,
  const volScalarField& pf,
  const volSymmTensorField& /*D*/
) const
{
  return dimensionedScalar("0.5", dimTime, 0.5)*pf*sin(phi_);
}
bool mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson::read()
{
  coeffDict_ <<= dict_.subDict(typeName + "Coeffs");
  Fr_.read(coeffDict_);
  eta_.read(coeffDict_);
  p_.read(coeffDict_);
  phi_.read(coeffDict_);
  phi_ *= constant::mathematical::pi/180.0;
  alphaDeltaMin_.read(coeffDict_);
  return true;
}
