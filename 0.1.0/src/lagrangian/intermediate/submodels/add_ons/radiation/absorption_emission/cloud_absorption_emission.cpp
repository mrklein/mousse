// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_absorption_emission.hpp"
#include "thermo_cloud.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace radiation
{
DEFINE_TYPE_NAME_AND_DEBUG(cloudAbsorptionEmission, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  absorptionEmissionModel,
  cloudAbsorptionEmission,
  dictionary
);
}
}
// Constructors 
mousse::radiation::cloudAbsorptionEmission::cloudAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel{dict, mesh},
  coeffsDict_{dict.subDict(typeName + "Coeffs")},
  cloudNames_{coeffsDict_.lookup("cloudNames")}
{}
// Destructor 
mousse::radiation::cloudAbsorptionEmission::~cloudAbsorptionEmission()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::cloudAbsorptionEmission::aDisp(const label) const
{
  tmp<volScalarField> ta
  {
    new volScalarField
    {
      // IOobject
      {
        "a",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      // dimensionedScalar("a", dimless/dimLength, 0.0)
      {"a", dimless/dimLength, 0.0}
    }
  };
  FOR_ALL(cloudNames_, i)
  {
    const thermoCloud& tc
    (
      mesh_.objectRegistry::lookupObject<thermoCloud>(cloudNames_[i])
    );
    ta() += tc.ap();
  }
  return ta;
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::cloudAbsorptionEmission::eDisp(const label /*bandI*/) const
{
  tmp<volScalarField> te
  {
    new volScalarField
    {
      // IOobject
      {
        "e",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      // dimensionedScalar("e", dimless/dimLength, 0.0)
      {"e", dimless/dimLength, 0.0}
    }
  };
  return te;
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::cloudAbsorptionEmission::EDisp(const label /*bandI*/) const
{
  tmp<volScalarField> tE
  {
    new volScalarField
    {
      // IOobject
      {
        "E",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      // dimensionedScalar("E", dimMass/dimLength/pow3(dimTime), 0.0)
      {"E", dimMass/dimLength/pow3(dimTime), 0.0}
    }
  };
  FOR_ALL(cloudNames_, i)
  {
    const thermoCloud& tc
    (
      mesh_.objectRegistry::lookupObject<thermoCloud>(cloudNames_[i])
    );
    tE() += tc.Ep();
  }
  // Total emission is 4 times the projected emission
  return 4*tE;
}
