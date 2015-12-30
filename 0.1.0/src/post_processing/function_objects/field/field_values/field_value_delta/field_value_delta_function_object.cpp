// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value_delta_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    fieldValueDeltaFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    fieldValueDeltaFunctionObject,
    dictionary
  );
}
