#ifndef CORE_PRIMITIVES_SYMM_TENSOR_2D_SYMM_TENSOR_2D_HPP_
#define CORE_PRIMITIVES_SYMM_TENSOR_2D_SYMM_TENSOR_2D_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   symm_tensor_2d.cpp
#include "_symm_tensor_2d.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SymmTensor2D<scalar> symmTensor2D;
//- Data associated with symmTensor2D type are contiguous
template<>
inline bool contiguous<symmTensor2D>() {return true;}
}  // namespace mousse
#endif
