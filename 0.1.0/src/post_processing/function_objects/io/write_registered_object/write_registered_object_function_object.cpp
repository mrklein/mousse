// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_registered_object_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    writeRegisteredObjectFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    writeRegisteredObjectFunctionObject,
    dictionary
  );
}
