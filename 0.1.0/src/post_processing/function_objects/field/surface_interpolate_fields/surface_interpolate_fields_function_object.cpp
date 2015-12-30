// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolate_fields_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    surfaceInterpolateFieldsFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    surfaceInterpolateFieldsFunctionObject,
    dictionary
  );
}
