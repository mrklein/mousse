// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   probes_function_object.cpp
#ifndef probes_function_object_hpp_
#define probes_function_object_hpp_
#include "probes.hpp"
#include "patch_probes.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<probes> probesFunctionObject;
  typedef OutputFilterFunctionObject<patchProbes> patchProbesFunctionObject;
}
#endif
