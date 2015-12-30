// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "probes_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug(probesFunctionObject, 0);
  defineNamedTemplateTypeNameAndDebug(patchProbesFunctionObject, 0);
  addToRunTimeSelectionTable
  (
    functionObject,
    probesFunctionObject,
    dictionary
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    patchProbesFunctionObject,
    dictionary
  );
}
