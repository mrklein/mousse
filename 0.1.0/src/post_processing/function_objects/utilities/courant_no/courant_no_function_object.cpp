// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "courant_no_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(CourantNoFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    CourantNoFunctionObject,
    dictionary
  );
}
