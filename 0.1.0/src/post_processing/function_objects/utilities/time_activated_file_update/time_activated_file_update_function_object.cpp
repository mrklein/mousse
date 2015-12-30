// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_activated_file_update_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    timeActivatedFileUpdateFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    timeActivatedFileUpdateFunctionObject,
    dictionary
  );
}
