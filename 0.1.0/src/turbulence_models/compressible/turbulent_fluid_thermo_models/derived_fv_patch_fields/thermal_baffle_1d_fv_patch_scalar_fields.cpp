// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle_1d_fv_patch_scalar_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
defineTemplateTypeNameAndDebugWithName
(
  mousse::compressible::constSolid_thermalBaffle1DFvPatchScalarField,
  "compressible::thermalBaffle1D<hConstSolidThermoPhysics>",
  0
);
defineTemplateTypeNameAndDebugWithName
(
  mousse::compressible::expoSolid_thermalBaffle1DFvPatchScalarField,
  "compressible::thermalBaffle1D<hPowerSolidThermoPhysics>",
  0
);
namespace mousse
{
namespace compressible
{
  addToPatchFieldRunTimeSelection
  (
    fvPatchScalarField,
    constSolid_thermalBaffle1DFvPatchScalarField
  );
  addToPatchFieldRunTimeSelection
  (
    fvPatchScalarField,
    expoSolid_thermalBaffle1DFvPatchScalarField
  );
}
}
