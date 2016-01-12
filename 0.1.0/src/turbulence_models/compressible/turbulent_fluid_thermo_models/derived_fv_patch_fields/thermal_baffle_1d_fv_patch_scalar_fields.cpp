// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle_1d_fv_patch_scalar_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  mousse::compressible::constSolid_thermalBaffle1DFvPatchScalarField,
  "compressible::thermalBaffle1D<hConstSolidThermoPhysics>",
  0
);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  mousse::compressible::expoSolid_thermalBaffle1DFvPatchScalarField,
  "compressible::thermalBaffle1D<hPowerSolidThermoPhysics>",
  0
);
namespace mousse
{
namespace compressible
{
ADD_TO_PATCH_FIELD_RUN_TIME_SELECTION
(
  fvPatchScalarField,
  constSolid_thermalBaffle1DFvPatchScalarField
);
ADD_TO_PATCH_FIELD_RUN_TIME_SELECTION
(
  fvPatchScalarField,
  expoSolid_thermalBaffle1DFvPatchScalarField
);
}
}
