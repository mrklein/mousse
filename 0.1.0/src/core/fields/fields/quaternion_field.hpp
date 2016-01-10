// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   quaternion_field.cpp
#ifndef quaternion_field_hpp_
#define quaternion_field_hpp_
#include "field.hpp"
#include "quaternion.hpp"
#define TEMPLATE
#include "field_functions_m.hpp"
namespace mousse
{
typedef Field<quaternion> quaternionField;
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#endif
