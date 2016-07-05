// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_convective_heat_transfer.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "kinematic_single_layer.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(mappedConvectiveHeatTransfer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  heatTransferModel,
  mappedConvectiveHeatTransfer,
  dictionary
);


// Constructors 
mappedConvectiveHeatTransfer::mappedConvectiveHeatTransfer
(
  surfaceFilmModel& owner,
  const dictionary& /*dict*/
)
:
  heatTransferModel{owner},
  htcConvPrimary_
  {
    {
      "htcConv",
      owner.time().timeName(),
      owner.primaryMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    owner.primaryMesh()
  },
  htcConvFilm_
  {
    {
      htcConvPrimary_.name(), // must have same name as above for mapping
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    owner.regionMesh(),
    {"zero", dimMass/pow3(dimTime)/dimTemperature, 0.0},
    owner.mappedPushedFieldPatchTypes<scalar>()
  }
{
  // Update the primary-side convective heat transfer coefficient
  htcConvPrimary_.correctBoundaryConditions();
  // Pull the data from the primary region via direct mapped BCs
  htcConvFilm_.correctBoundaryConditions();
}


// Destructor 
mappedConvectiveHeatTransfer::~mappedConvectiveHeatTransfer()
{}


// Member Functions 
void mappedConvectiveHeatTransfer::correct()
{
  // Update the primary-side convective heat transfer coefficient
  htcConvPrimary_.correctBoundaryConditions();
  // Pull the data from the primary region via direct mapped BCs
  htcConvFilm_.correctBoundaryConditions();
}


tmp<volScalarField> mappedConvectiveHeatTransfer::h() const
{
  return htcConvFilm_;
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

