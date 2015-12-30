// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_source_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    cellSourceFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    cellSourceFunctionObject,
    dictionary
  );
}
