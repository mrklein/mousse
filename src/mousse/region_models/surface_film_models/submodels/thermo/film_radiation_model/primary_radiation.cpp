// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primary_radiation.hpp"
#include "vol_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(primaryRadiation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmRadiationModel,
  primaryRadiation,
  dictionary
);


// Constructors 
primaryRadiation::primaryRadiation
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmRadiationModel{typeName, owner, dict},
  QinPrimary_
  {
    {
      "Qin", // same name as Qin on primary region to enable mapping
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    owner.regionMesh(),
    {"zero", dimMass/pow3(dimTime), 0.0},
    owner.mappedPushedFieldPatchTypes<scalar>()
  }
{}


// Destructor 
primaryRadiation::~primaryRadiation()
{}


// Member Functions 
void primaryRadiation::correct()
{
  // Transfer Qin from primary region
  QinPrimary_.correctBoundaryConditions();
}


tmp<volScalarField> primaryRadiation::Shs()
{
  tmp<volScalarField> tShs
  {
    new volScalarField
    {
      {
        typeName + ":Shs",
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"zero", dimMass/pow3(dimTime), 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  scalarField& Shs = tShs();
  const scalarField& QinP = QinPrimary_.internalField();
  const scalarField& alpha = owner_.alpha().internalField();
  Shs = QinP*alpha;
  return tShs;
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

