// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "partial_write_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    partialWriteFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    partialWriteFunctionObject,
    dictionary
  );
}
