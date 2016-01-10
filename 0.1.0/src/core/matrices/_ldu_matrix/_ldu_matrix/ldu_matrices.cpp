// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_matrix.hpp"
#include "field_types.hpp"

namespace mousse {

MAKE_LDU_MATRIX(scalar, scalar, scalar);
MAKE_LDU_MATRIX(vector, scalar, scalar);
MAKE_LDU_MATRIX(sphericalTensor, scalar, scalar);
MAKE_LDU_MATRIX(symmTensor, scalar, scalar);
MAKE_LDU_MATRIX(tensor, scalar, scalar);

};
