// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_transport_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(scalarTransportFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    scalarTransportFunctionObject,
    dictionary
  );
}
