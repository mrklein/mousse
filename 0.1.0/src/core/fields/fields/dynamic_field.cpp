// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_field.hpp"
// IOstream Operators 
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField(Istream& is)
:
  Field<T>(is),
  capacity_(Field<T>::size())
{}
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::tmp<mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv> >
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::clone() const
{
  return tmp<DynamicField<T, SizeInc, SizeMult, SizeDiv> >
  (
    new DynamicField<T, SizeInc, SizeMult, SizeDiv>(*this)
  );
}
// IOstream Operator
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const DynamicField<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
  os << static_cast<const Field<T>&>(lst);
  return os;
}
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::Istream& mousse::operator>>
(
  Istream& is,
  DynamicField<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
  is >> static_cast<Field<T>&>(lst);
  lst.capacity_ = lst.Field<T>::size();
  return is;
}
