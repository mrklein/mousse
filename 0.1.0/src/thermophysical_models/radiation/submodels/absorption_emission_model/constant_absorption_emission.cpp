// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_absorption_emission.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(constantAbsorptionEmission, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  absorptionEmissionModel,
  constantAbsorptionEmission,
  dictionary
);

}
}


// Constructors 
mousse::radiation::constantAbsorptionEmission::constantAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel{dict, mesh},
  coeffsDict_{dict.subDict(typeName + "Coeffs")},
  a_{coeffsDict_.lookup("absorptivity")},
  e_{coeffsDict_.lookup("emissivity")},
  E_{coeffsDict_.lookup("E")}
{}


// Destructor 
mousse::radiation::constantAbsorptionEmission::~constantAbsorptionEmission()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::constantAbsorptionEmission::aCont(const label /*bandI*/) const
{
  tmp<volScalarField> ta
  {
    new volScalarField
    {
      {
        "a",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      a_
    }
  };

  return ta;
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::constantAbsorptionEmission::eCont(const label /*bandI*/) const
{
  tmp<volScalarField> te
  {
    new volScalarField
    {
      {
        "e",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      e_
    }
  };

  return te;
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::constantAbsorptionEmission::ECont(const label /*bandI*/) const
{
  tmp<volScalarField> tE
  {
    new volScalarField
    {
      {
        "E",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      E_
    }
  };

  return tE;
}

