// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  const AveragingMethod<Type>& x
)
{
  FieldField<Field, Type>::operator=(x);
  updateGrad();
}
template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  const Type& x
)
{
  FieldField<Field, Type>::operator=(x);
  updateGrad();
}
template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  tmp<FieldField<Field, Type> > x
)
{
  FieldField<Field, Type>::operator=(x());
  updateGrad();
}
template<class Type>
inline void mousse::AveragingMethod<Type>::operator+=
(
  tmp<FieldField<Field, Type> > x
)
{
  FieldField<Field, Type>::operator+=(x());
  updateGrad();
}
template<class Type>
inline void mousse::AveragingMethod<Type>::operator*=
(
  tmp<FieldField<Field, Type> > x
)
{
  FieldField<Field, Type>::operator*=(x());
  updateGrad();
}
template<class Type>
inline void mousse::AveragingMethod<Type>::operator/=
(
  tmp<FieldField<Field, scalar> > x
)
{
  FieldField<Field, Type>::operator/=(x());
  updateGrad();
}
