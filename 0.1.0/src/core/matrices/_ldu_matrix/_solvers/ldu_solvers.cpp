// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pcicg.hpp"
#include "pbicccg.hpp"
#include "pbicicg.hpp"
#include "_smooth_solver.hpp"
#include "_diagonal_solver.hpp"
#include "field_types.hpp"


#define MAKE_LDU_SOLVERS(Type, DType, LUType)                                 \
                                                                              \
  MAKE_LDU_SOLVER(DiagonalSolver, Type, DType, LUType);                       \
  MAKE_LDU_SYM_SOLVER(DiagonalSolver, Type, DType, LUType);                   \
  MAKE_LDU_ASYM_SOLVER(DiagonalSolver, Type, DType, LUType);                  \
                                                                              \
  MAKE_LDU_SOLVER(PCICG, Type, DType, LUType);                                \
  MAKE_LDU_SYM_SOLVER(PCICG, Type, DType, LUType);                            \
                                                                              \
  MAKE_LDU_SOLVER(PBiCCCG, Type, DType, LUType);                              \
  MAKE_LDU_ASYM_SOLVER(PBiCCCG, Type, DType, LUType);                         \
                                                                              \
  MAKE_LDU_SOLVER(PBiCICG, Type, DType, LUType);                              \
  MAKE_LDU_ASYM_SOLVER(PBiCICG, Type, DType, LUType);                         \
                                                                              \
  MAKE_LDU_SOLVER(SmoothSolver, Type, DType, LUType);                         \
  MAKE_LDU_SYM_SOLVER(SmoothSolver, Type, DType, LUType);                     \
  MAKE_LDU_ASYM_SOLVER(SmoothSolver, Type, DType, LUType);


namespace mousse {

MAKE_LDU_SOLVERS(scalar, scalar, scalar);
MAKE_LDU_SOLVERS(vector, scalar, scalar);
MAKE_LDU_SOLVERS(sphericalTensor, scalar, scalar);
MAKE_LDU_SOLVERS(symmTensor, scalar, scalar);
MAKE_LDU_SOLVERS(tensor, scalar, scalar);

};
