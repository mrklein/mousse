// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_dictionary_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(writeDictionaryFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    writeDictionaryFunctionObject,
    dictionary
  );
}
