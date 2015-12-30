// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_factor_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(blendingFactorFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    blendingFactorFunctionObject,
    dictionary
  );
}
