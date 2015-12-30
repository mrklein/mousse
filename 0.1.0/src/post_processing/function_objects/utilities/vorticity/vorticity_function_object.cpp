// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vorticity_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(vorticityFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    vorticityFunctionObject,
    dictionary
  );
}
