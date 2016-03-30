#ifndef CORE_PRIMITIVES_DIAG_TENSOR_DIAG_TENSOR_HPP_
#define CORE_PRIMITIVES_DIAG_TENSOR_DIAG_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_diag_tensor.hpp"
#include "contiguous.hpp"


namespace mousse {

typedef DiagTensor<scalar> diagTensor;

//- Data associated with diagTensor type are contiguous
template<> inline bool contiguous<diagTensor>() {return true;}

}  // namespace mousse

#endif
