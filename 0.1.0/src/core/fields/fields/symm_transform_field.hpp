#ifndef CORE_FIELDS_FIELDS_SYMM_TRANSFORM_FIELD_HPP_
#define CORE_FIELDS_FIELDS_SYMM_TRANSFORM_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   symm_transform_field.cpp
#include "symm_transform.hpp"
#include "symm_tensor_field.hpp"
#include "spherical_tensor.hpp"
namespace mousse
{
template<class Type>
void transform(Field<Type>&, const symmTensorField&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const symmTensorField&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const symmTensorField&, const tmp<Field<Type> >&);
template<class Type>
tmp<Field<Type> > transform(const tmp<symmTensorField>&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform
(
  const tmp<symmTensorField>&,
  const tmp<Field<Type> >&
);
template<class Type>
void transform(Field<Type>&, const tensor&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensor&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensor&, const tmp<Field<Type> >&);
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const symmTensorField&
);
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tmp<symmTensorField>&
);
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const symmTensorField&
);
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tmp<symmTensorField>&
);
template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
  const symmTensorField&
);
template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
  const tmp<symmTensorField>&
);
}  // namespace mousse
#ifdef NoRepository
#   include "symm_transform_field.cpp"
#endif
#endif
