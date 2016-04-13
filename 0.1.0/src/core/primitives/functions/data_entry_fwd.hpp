#ifndef CORE_PRIMITIVES_FUNCTIONS_DATA_ENTRY_FWD_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_DATA_ENTRY_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"
#include "vector.hpp"
#include "symm_tensor.hpp"
#include "spherical_tensor.hpp"
#include "tensor.hpp"


namespace mousse {

typedef DataEntry<label> labelDataEntry;
typedef DataEntry<scalar> scalarDataEntry;
typedef DataEntry<vector> vectorDataEntry;
typedef DataEntry<symmTensor> symmTensorDataEntry;
typedef DataEntry<sphericalTensor> sphericalTensorDataEntry;
typedef DataEntry<tensor> tensorDataEntry;

}

#endif
