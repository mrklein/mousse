// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(sampledSetsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    sampledSetsFunctionObject,
    dictionary
  );
}
