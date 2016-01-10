// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   symm_tensor.cpp
#ifndef symm_tensor_hpp_
#define symm_tensor_hpp_
#include "_symm_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SymmTensor<scalar> symmTensor;
//- Data associated with symmTensor type are contiguous
template<>
inline bool contiguous<symmTensor>() {return true;}
}  // namespace mousse
#endif
