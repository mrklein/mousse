#ifndef POST_PROCESSING_POST_CALC_CALC_HPP_
#define POST_PROCESSING_POST_CALC_CALC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"


namespace mousse {

void calc(const argList& args, const Time& runTime, const fvMesh& mesh);

}

#endif

