#ifndef CORE_FIELDS_FIELDS_QUATERNION_FIELD_HPP_
#define CORE_FIELDS_FIELDS_QUATERNION_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field.hpp"
#include "quaternion.hpp"
#define TEMPLATE
#include "field_functions_m.inc"


namespace mousse {

typedef Field<quaternion> quaternionField;

}  // namespace mousse

#include "undef_field_functions_m.inc"

#endif
