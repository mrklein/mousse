// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "filter_function_object_template.hpp"
// Global Functions 
extern "C"
{
  // dynamicCode:
  // SHA1 = ${SHA1sum}
  //
  // unique function name that can be checked if the correct library version
  // has been loaded
  void ${typeName}_${SHA1sum}(bool load)
  {
    if (load)
    {
      // code that can be explicitly executed after loading
    }
    else
    {
      // code that can be explicitly executed before unloading
    }
  }
}
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(${typeName}FilterFunctionObject, 0);
  //addToRunTimeSelectionTable
  addRemovableToRunTimeSelectionTable
  (
    functionObject,
    ${typeName}FilterFunctionObject,
    dictionary
  );
}
