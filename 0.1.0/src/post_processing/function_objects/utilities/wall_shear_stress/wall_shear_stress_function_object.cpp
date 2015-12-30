// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_shear_stress_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(wallShearStressFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    wallShearStressFunctionObject,
    dictionary
  );
}
