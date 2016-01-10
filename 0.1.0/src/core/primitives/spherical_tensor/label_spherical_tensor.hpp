// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   label_spherical_tensor.cpp
#ifndef label_spherical_tensor_hpp_
#define label_spherical_tensor_hpp_
#include "_spherical_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SphericalTensor<label> labelSphericalTensor;
//- Identity labelTensor
static const labelSphericalTensor labelI(1);
//- Data associated with labelSphericalTensor type are contiguous
template<>
inline bool contiguous<labelSphericalTensor>() {return true;}
}  // namespace mousse
#endif
