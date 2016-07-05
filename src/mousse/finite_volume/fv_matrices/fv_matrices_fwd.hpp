#ifndef FINITE_VOLUME_FV_MATRICES_FV_MATRICES_FWD_HPP_
#define FINITE_VOLUME_FV_MATRICES_FV_MATRICES_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class fvMatrix;

typedef fvMatrix<scalar> fvScalarMatrix;
typedef fvMatrix<vector> fvVectorMatrix;
typedef fvMatrix<sphericalTensor> fvSphericalTensorMatrix;
typedef fvMatrix<symmTensor> fvSymmTensorMatrix;
typedef fvMatrix<tensor> fvTensorMatrix;

}  // namespace mousse

#endif

