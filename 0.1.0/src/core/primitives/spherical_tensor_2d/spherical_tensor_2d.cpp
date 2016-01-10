// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_tensor_2d.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const sphericalTensor2D::typeName = "sphericalTensor2D";
template<>
const char* sphericalTensor2D::componentNames[] = {"ii"};
template<>
const sphericalTensor2D sphericalTensor2D::zero(0);
template<>
const sphericalTensor2D sphericalTensor2D::one(1);
template<>
const sphericalTensor2D sphericalTensor2D::max(VGREAT);
template<>
const sphericalTensor2D sphericalTensor2D::min(-VGREAT);
template<>
const sphericalTensor2D sphericalTensor2D::I(1);
template<>
const sphericalTensor2D sphericalTensor2D::oneThirdI(1.0/3.0);
template<>
const sphericalTensor2D sphericalTensor2D::twoThirdsI(2.0/3.0);
}  // namespace mousse
