// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tgauss_seidel_smoother.hpp"
#include "field_types.hpp"


#define MAKE_LDU_SMOOTHERS(Type, DType, LUType)                               \
                                                                              \
  MAKE_LDU_SMOOTHER(TGaussSeidelSmoother, Type, DType, LUType);               \
  MAKE_LDU_SYM_SMOOTHER(TGaussSeidelSmoother, Type, DType, LUType);           \
  MAKE_LDU_ASYM_SMOOTHER(TGaussSeidelSmoother, Type, DType, LUType);


namespace mousse {

MAKE_LDU_SMOOTHERS(scalar, scalar, scalar);
MAKE_LDU_SMOOTHERS(vector, scalar, scalar);
MAKE_LDU_SMOOTHERS(sphericalTensor, scalar, scalar);
MAKE_LDU_SMOOTHERS(symmTensor, scalar, scalar);
MAKE_LDU_SMOOTHERS(tensor, scalar, scalar);

};
