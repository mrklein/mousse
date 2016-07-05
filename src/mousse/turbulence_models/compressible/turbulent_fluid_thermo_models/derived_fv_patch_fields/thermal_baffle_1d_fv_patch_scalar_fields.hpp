#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_THERMAL_BAFFLE_1D_FV_PATCH_SCALAR_FIELDS_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_THERMAL_BAFFLE_1D_FV_PATCH_SCALAR_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle_1d_fv_patch_scalar_field.hpp"
#include "solid_thermo_physics_types.hpp"


namespace mousse {
namespace compressible {

typedef thermalBaffle1DFvPatchScalarField<hConstSolidThermoPhysics>
  constSolid_thermalBaffle1DFvPatchScalarField;

typedef thermalBaffle1DFvPatchScalarField<hPowerSolidThermoPhysics>
  expoSolid_thermalBaffle1DFvPatchScalarField;

}
}  // namespace mousse

#endif

