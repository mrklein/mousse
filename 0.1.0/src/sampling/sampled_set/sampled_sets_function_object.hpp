#ifndef SAMPLING_SAMPLED_SET_SAMPLED_SETS_FUNCTION_OBJECT_HPP_
#define SAMPLING_SAMPLED_SET_SAMPLED_SETS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<sampledSets> sampledSetsFunctionObject;

}

#endif
