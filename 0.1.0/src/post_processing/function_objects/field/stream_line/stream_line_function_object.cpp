// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stream_line_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(streamLineFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    streamLineFunctionObject,
    dictionary
  );
}
