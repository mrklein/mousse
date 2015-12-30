// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "remove_registered_object_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    removeRegisteredObjectFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    removeRegisteredObjectFunctionObject,
    dictionary
  );
}
