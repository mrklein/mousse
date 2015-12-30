// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_fvc_grad_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(calcFvcGradFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    calcFvcGradFunctionObject,
    dictionary
  );
}
