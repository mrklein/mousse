// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lambda2_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(Lambda2FunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    Lambda2FunctionObject,
    dictionary
  );
}
