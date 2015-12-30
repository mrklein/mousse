// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "forces_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(forcesFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    forcesFunctionObject,
    dictionary
  );
}
