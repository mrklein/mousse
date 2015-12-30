// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_field_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(processorFieldFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    processorFieldFunctionObject,
    dictionary
  );
}
