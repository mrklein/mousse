// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_bounded_stream_line_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(wallBoundedStreamLineFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    wallBoundedStreamLineFunctionObject,
    dictionary
  );
}
