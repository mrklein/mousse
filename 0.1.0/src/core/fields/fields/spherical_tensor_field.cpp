// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_tensor_field.hpp"
#include "transform_field.hpp"
#define TEMPLATE
#include "field_functions_m.cpp"
namespace mousse
{
// global functions 
UNARY_FUNCTION(scalar, sphericalTensor, tr)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, sph)
UNARY_FUNCTION(scalar, sphericalTensor, det)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, inv)
BINARY_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, divide)
BINARY_TYPE_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, divide)
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tensorField& tf
)
{
  return sph(tf);
}
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tmp<tensorField>& ttf
)
{
  tmp<Field<sphericalTensor> > ret =
    transformFieldMask<sphericalTensor>(ttf());
  ttf.clear();
  return ret;
}
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const symmTensorField& stf
)
{
  return sph(stf);
}
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tmp<symmTensorField>& tstf
)
{
  tmp<Field<sphericalTensor> > ret =
    transformFieldMask<sphericalTensor>(tstf());
  tstf.clear();
  return ret;
}
}  // namespace mousse
#include "undef_field_functions_m.hpp"
