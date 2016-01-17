// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solver_performance.hpp"
#include "field_types.hpp"
namespace mousse {

MAKE_SOLVER_PERFORMANCE(scalar);
MAKE_SOLVER_PERFORMANCE(vector);
MAKE_SOLVER_PERFORMANCE(sphericalTensor);
MAKE_SOLVER_PERFORMANCE(symmTensor);
MAKE_SOLVER_PERFORMANCE(tensor);

};
