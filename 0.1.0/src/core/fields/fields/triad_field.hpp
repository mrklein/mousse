// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   triad_field.cpp
#ifndef triad_field_hpp_
#define triad_field_hpp_
#include "field.hpp"
#include "triad.hpp"
#define TEMPLATE
#include "field_functions_m.hpp"
namespace mousse
{
typedef Field<triad> triadField;
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#endif
