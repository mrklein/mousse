#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_WAVE_TRANSMISSIVE_FV_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_WAVE_TRANSMISSIVE_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class waveTransmissiveFvPatchField;
MAKE_PATCH_TYPE_FIELD_TYPEDEFS(waveTransmissive);

}  // namespace mousse

#endif
