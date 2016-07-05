#ifndef CORE_FIELDS_FIELDS_TRIAD_FIELD_HPP_
#define CORE_FIELDS_FIELDS_TRIAD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field.hpp"
#include "triad.hpp"
#define TEMPLATE
#include "field_functions_m.inc"


namespace mousse {

typedef Field<triad> triadField;

}  // namespace mousse

#include "undef_field_functions_m.inc"

#endif
