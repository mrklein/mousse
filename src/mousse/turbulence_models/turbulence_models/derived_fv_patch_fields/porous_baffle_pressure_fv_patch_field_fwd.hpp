#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_POROUS_BAFFLE_PRESSURE_FV_PATCH_FIELDS_FWD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_POROUS_BAFFLE_PRESSURE_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class porousBafflePressureFvPatchField;
MAKE_PATCH_TYPE_FIELD_TYPEDEFS(porousBafflePressure);

}  // namespace mousse

#endif

