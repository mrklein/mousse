// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_radiation.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noRadiation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmRadiationModel,
  noRadiation,
  dictionary
);


// Constructors 
noRadiation::noRadiation
(
  surfaceFilmModel& owner,
  const dictionary& /*dict*/
)
:
  filmRadiationModel{owner}
{}


// Destructor 
noRadiation::~noRadiation()
{}


// Member Functions 
void noRadiation::correct()
{
  // do nothing
}


tmp<volScalarField> noRadiation::Shs()
{
  return tmp<volScalarField>
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
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

