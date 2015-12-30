// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_average_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(fieldAverageFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    fieldAverageFunctionObject,
    dictionary
  );
}
