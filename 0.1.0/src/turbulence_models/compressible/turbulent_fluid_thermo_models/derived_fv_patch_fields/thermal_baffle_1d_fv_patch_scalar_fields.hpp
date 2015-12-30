// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef thermal_baffle_1d_fv_patch_scalar_fields_hpp_
#define thermal_baffle_1d_fv_patch_scalar_fields_hpp_
#include "thermal_baffle_1d_fv_patch_scalar_field.hpp"
#include "solid_thermo_physics_types.hpp"
namespace mousse
{
namespace compressible
{
  typedef thermalBaffle1DFvPatchScalarField
  <
    hConstSolidThermoPhysics
  >constSolid_thermalBaffle1DFvPatchScalarField;
  typedef thermalBaffle1DFvPatchScalarField
  <
    hPowerSolidThermoPhysics
  >expoSolid_thermalBaffle1DFvPatchScalarField;
}
}  // namespace mousse
#endif
