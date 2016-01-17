// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   transform_field.cpp
#ifndef transform_field_hpp_
#define transform_field_hpp_
#include "transform.hpp"
#include "quaternion.hpp"
#include "septernion.hpp"
#include "vector_field.hpp"
#include "tensor_field.hpp"
namespace mousse
{
template<class Type>
void transform(Field<Type>&, const tensorField&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensorField&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensorField&, const tmp<Field<Type> >&);
template<class Type>
tmp<Field<Type> > transform(const tmp<tensorField>&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tmp<tensorField>&, const tmp<Field<Type> >&);
template<class Type>
void transform(Field<Type>&, const tensor&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensor&, const Field<Type>&);
template<class Type>
tmp<Field<Type> > transform(const tensor&, const tmp<Field<Type> >&);
template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const Field<Type2>&);
template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const tmp<Field<Type2> >&);
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tensorField&
);
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tmp<tensorField>&
);
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tensorField&
);
template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
  const tmp<tensorField>&
);
//- Rotate given vectorField with the given quaternion
void transform(vectorField&, const quaternion&, const vectorField&);
//- Rotate given vectorField with the given quaternion
tmp<vectorField> transform(const quaternion&, const vectorField&);
//- Rotate given tmp<vectorField> with the given quaternion
tmp<vectorField> transform(const quaternion&, const tmp<vectorField>&);
//- Transform given vectorField with the given septernion
void transform(vectorField&, const septernion&, const vectorField&);
//- Transform given vectorField with the given septernion
tmp<vectorField> transform(const septernion&, const vectorField&);
//- Transform given tmp<vectorField> with the given septernion
tmp<vectorField> transform(const septernion&, const tmp<vectorField>&);
}  // namespace mousse
#ifdef NoRepository
#   include "transform_field_templates.cpp"
#endif
#endif
