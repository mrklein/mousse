// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_min_max_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(fieldMinMaxFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    fieldMinMaxFunctionObject,
    dictionary
  );
}
