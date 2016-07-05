#ifndef CORE_FIELDS_FIELD_FIELDS_TRANSFORM_FIELD_FIELD_HPP_
#define CORE_FIELDS_FIELD_FIELDS_TRANSFORM_FIELD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform.hpp"
#include "tensor_field_field.hpp"


namespace mousse {

template<template<class> class Field, class Type>
void transform
(
  FieldField<Field, Type>&,
  const FieldField<Field, tensor>&,
  const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const FieldField<Field, tensor>&,
  const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const FieldField<Field, tensor>&,
  const tmp<FieldField<Field, Type> >&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const tmp<FieldField<Field, tensor> >&,
  const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const tmp<FieldField<Field, tensor> >&,
  const tmp<FieldField<Field, Type> >&
);

template<template<class> class Field, class Type>
void transform
(
  FieldField<Field, Type>&,
  const tensor&,
  const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const tensor&,
  const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > transform
(
  const tensor&,
  const tmp<FieldField<Field, Type> >&
);

}  // namespace mousse


#include "transform_field_field.ipp"

#endif
