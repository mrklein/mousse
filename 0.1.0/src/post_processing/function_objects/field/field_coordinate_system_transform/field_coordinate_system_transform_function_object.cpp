// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_coordinate_system_transform_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    fieldCoordinateSystemTransformFunctionObject, 0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    fieldCoordinateSystemTransformFunctionObject,
    dictionary
  );
}
