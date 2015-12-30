// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_fields_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    nearWallFieldsFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    nearWallFieldsFunctionObject,
    dictionary
  );
}
