// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   diag_tensor.cpp
#ifndef diag_tensor_hpp_
#define diag_tensor_hpp_
#include "_diag_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef DiagTensor<scalar> diagTensor;
//- Data associated with diagTensor type are contiguous
template<>
inline bool contiguous<diagTensor>() {return true;}
}  // namespace mousse
#endif
