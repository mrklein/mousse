// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_fields_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(dsmcFieldsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    dsmcFieldsFunctionObject,
    dictionary
  );
}
