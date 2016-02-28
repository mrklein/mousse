// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "schaeffer_frictional_stress.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace frictionalStressModels {
DEFINE_TYPE_NAME_AND_DEBUG(Schaeffer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  frictionalStressModel,
  Schaeffer,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::Schaeffer
(
  const dictionary& dict
)
:
  frictionalStressModel{dict},
  coeffDict_{dict.subDict(typeName + "Coeffs")},
  phi_{"phi", dimless, coeffDict_}
{
  phi_ *= constant::mathematical::pi/180.0;
}
// Destructor 
mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::~Schaeffer()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::
frictionalPressure
(
  const volScalarField& alpha1,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& /*alphaMax*/
) const
{
  return
    dimensionedScalar{"1e24", {1, -1, -2, 0, 0}, 1e24}
    *pow(mousse::max(alpha1 - alphaMinFriction, scalar(0)), 10.0);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::
frictionalPressurePrime
(
  const volScalarField& alpha1,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& /*alphaMax*/
) const
{
  return
    dimensionedScalar{"1e25", {1, -1, -2, 0, 0}, 1e25}
    *pow(mousse::max(alpha1 - alphaMinFriction, scalar(0)), 9.0);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::nu
(
  const volScalarField& alpha1,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& /*alphaMax*/,
  const volScalarField& pf,
  const volSymmTensorField& D
) const
{
  const scalar I2Dsmall = 1.0e-15;
  // Creating nu assuming it should be 0 on the boundary which may not be
  // true
  tmp<volScalarField> tnu
  {
    new volScalarField
    {
      {
        "Schaeffer:nu",
        alpha1.mesh().time().timeName(),
        alpha1.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      alpha1.mesh(),
      {"nu", {0, 2, -1, 0, 0}, 0.0}
    }
  };
  volScalarField& nuf = tnu();
  FOR_ALL(D, celli)
  {
    if (alpha1[celli] > alphaMinFriction.value())
    {
      nuf[celli] =
        0.5*pf[celli]*sin(phi_.value())
       /(
          sqrt(1.0/6.0*(sqr(D[celli].xx() - D[celli].yy())
         + sqr(D[celli].yy() - D[celli].zz())
         + sqr(D[celli].zz() - D[celli].xx()))
         + sqr(D[celli].xy()) + sqr(D[celli].xz())
         + sqr(D[celli].yz())) + I2Dsmall
        );
    }
  }
  // Correct coupled BCs
  nuf.correctBoundaryConditions();
  return tnu;
}
bool mousse::kineticTheoryModels::frictionalStressModels::Schaeffer::read()
{
  coeffDict_ <<= dict_.subDict(typeName + "Coeffs");
  phi_.read(coeffDict_);
  phi_ *= constant::mathematical::pi/180.0;
  return true;
}
