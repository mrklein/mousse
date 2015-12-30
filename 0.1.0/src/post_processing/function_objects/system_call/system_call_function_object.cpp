// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "system_call_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(systemCallFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    systemCallFunctionObject,
    dictionary
  );
}
