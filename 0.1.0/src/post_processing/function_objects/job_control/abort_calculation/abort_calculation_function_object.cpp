// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "abort_calculation_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(abortCalculationFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    abortCalculationFunctionObject,
    dictionary
  );
}
