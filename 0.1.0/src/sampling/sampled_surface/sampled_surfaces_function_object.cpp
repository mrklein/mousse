// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surfaces_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(sampledSurfacesFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    sampledSurfacesFunctionObject,
    dictionary
  );
}
