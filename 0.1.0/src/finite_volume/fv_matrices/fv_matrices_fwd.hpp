// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef fv_matrices_fwd_hpp_
#define fv_matrices_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type>
class fvMatrix;
typedef fvMatrix<scalar> fvScalarMatrix;
typedef fvMatrix<vector> fvVectorMatrix;
typedef fvMatrix<sphericalTensor> fvSphericalTensorMatrix;
typedef fvMatrix<symmTensor> fvSymmTensorMatrix;
typedef fvMatrix<tensor> fvTensorMatrix;
}  // namespace mousse
#endif
