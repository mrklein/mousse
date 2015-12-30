// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "residuals_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(residualsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    residualsFunctionObject,
    dictionary
  );
}
