// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "peclet_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(PecletFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    PecletFunctionObject,
    dictionary
  );
}
