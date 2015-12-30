// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "y_plus_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(yPlusFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    yPlusFunctionObject,
    dictionary
  );
}
