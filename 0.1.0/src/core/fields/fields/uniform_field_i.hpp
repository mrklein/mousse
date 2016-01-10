// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_field.hpp"
template<class Type>
inline mousse::UniformField<Type>::UniformField(const Type& value)
:
  value_(value)
{}
template<class Type>
inline Type mousse::UniformField<Type>::operator[](const label) const
{
  return value_;
}
template<class Type>
inline mousse::UniformField<Type> mousse::UniformField<Type>::field() const
{
  return UniformField(value_);
}
