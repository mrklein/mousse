// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef primitive_fields_fwd_hpp_
#define primitive_fields_fwd_hpp_

#include "field_types.hpp"

namespace mousse
{
template<class Type> class Field;
typedef Field<label> labelField;
typedef Field<scalar> scalarField;
typedef Field<vector> vectorField;
typedef Field<sphericalTensor> sphericalTensorField;
typedef Field<symmTensor> symmTensorField;
typedef Field<tensor> tensorField;
}  // namespace mousse

#endif
