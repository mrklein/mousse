// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "q_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(QFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    QFunctionObject,
    dictionary
  );
}
