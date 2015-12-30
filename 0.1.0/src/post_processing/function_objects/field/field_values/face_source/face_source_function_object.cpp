// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_source_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    faceSourceFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    faceSourceFunctionObject,
    dictionary
  );
}
