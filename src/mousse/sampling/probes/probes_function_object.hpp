#ifndef SAMPLING_PROBES_PROBES_FUNCTION_OBJECT_HPP_
#define SAMPLING_PROBES_PROBES_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "probes.hpp"
#include "patch_probes.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<probes> probesFunctionObject;
typedef OutputFilterFunctionObject<patchProbes> patchProbesFunctionObject;

}

#endif

