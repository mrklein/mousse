// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_fields_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(readFieldsFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    readFieldsFunctionObject,
    dictionary
  );
}
