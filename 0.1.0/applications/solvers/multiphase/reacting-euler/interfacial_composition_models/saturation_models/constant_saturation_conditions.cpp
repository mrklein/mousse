// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_saturation_conditions.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace saturationModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantSaturationConditions, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  saturationModel,
  constantSaturationConditions,
  dictionary
);
}
}
// Constructors 
mousse::saturationModels::constantSaturationConditions::
constantSaturationConditions(const dictionary& dict)
:
  saturationModel{},
  pSat_{"pSat", dimPressure, dict},
  Tsat_{"Tsat", dimTemperature, dict}
{}
// Destructor 
mousse::saturationModels::constantSaturationConditions::
~constantSaturationConditions()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::constantSaturationConditions::pSat
(
  const volScalarField& T
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "pSat",
        T.mesh().time().timeName(),
        T.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      T.mesh(),
      pSat_
    }
  };
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::constantSaturationConditions::pSatPrime
(
  const volScalarField& T
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "pSatPrime",
        T.mesh().time().timeName(),
        T.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      T.mesh(),
      {"zero", dimPressure/dimTemperature, 0}
    }
  };
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::constantSaturationConditions::lnPSat
(
  const volScalarField& T
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "lnPSat",
        T.mesh().time().timeName(),
        T.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      T.mesh(),
      {"lnPSat", dimless, log(pSat_.value())}
    }
  };
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::constantSaturationConditions::Tsat
(
  const volScalarField& p
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "Tsat",
        p.mesh().time().timeName(),
        p.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      p.mesh(),
      Tsat_
    }
  };
}
