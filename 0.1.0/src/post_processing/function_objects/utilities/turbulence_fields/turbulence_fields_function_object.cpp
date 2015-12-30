// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulence_fields_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(turbulenceFieldsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    turbulenceFieldsFunctionObject,
    dictionary
  );
}
