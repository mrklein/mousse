// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_preconditioner.hpp"
#include "_diagonal_preconditioner.hpp"
#include "tdilu_preconditioner.hpp"
#include "field_types.hpp"


#define MAKE_LDU_PRECONDITIONERS(Type, DType, LUType)                         \
                                                                              \
  MAKE_LDU_PRECONDITIONER(NoPreconditioner, Type, DType, LUType);             \
  MAKE_LDU_SYM_PRECONDITIONER(NoPreconditioner, Type, DType, LUType);         \
  MAKE_LDU_ASYM_PRECONDITIONER(NoPreconditioner, Type, DType, LUType);        \
                                                                              \
  MAKE_LDU_PRECONDITIONER(DiagonalPreconditioner, Type, DType, LUType);       \
  MAKE_LDU_SYM_PRECONDITIONER(DiagonalPreconditioner, Type, DType, LUType);   \
  MAKE_LDU_ASYM_PRECONDITIONER(DiagonalPreconditioner, Type, DType, LUType);  \
                                                                              \
  MAKE_LDU_PRECONDITIONER(TDILUPreconditioner, Type, DType, LUType);          \
  MAKE_LDU_ASYM_PRECONDITIONER(TDILUPreconditioner, Type, DType, LUType);


namespace mousse {

MAKE_LDU_PRECONDITIONERS(scalar, scalar, scalar);
MAKE_LDU_PRECONDITIONERS(vector, scalar, scalar);
MAKE_LDU_PRECONDITIONERS(sphericalTensor, scalar, scalar);
MAKE_LDU_PRECONDITIONERS(symmTensor, scalar, scalar);
MAKE_LDU_PRECONDITIONERS(tensor, scalar, scalar);

};
