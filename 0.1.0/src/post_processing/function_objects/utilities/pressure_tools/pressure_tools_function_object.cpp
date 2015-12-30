// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_tools_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(pressureToolsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    pressureToolsFunctionObject,
    dictionary
  );
}
