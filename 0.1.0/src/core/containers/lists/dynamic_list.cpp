// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_list.hpp"
// IOstream Operators 
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList(Istream& is)
:
  List<T>(is),
  capacity_(List<T>::size())
{}
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
  os << static_cast<const List<T>&>(lst);
  return os;
}
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
mousse::Istream& mousse::operator>>
(
  Istream& is,
  DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
  is >> static_cast<List<T>&>(lst);
  lst.capacity_ = lst.List<T>::size();
  return is;
}
