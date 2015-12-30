// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "force_coeffs_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(forceCoeffsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    forceCoeffsFunctionObject,
    dictionary
  );
}
