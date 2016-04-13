// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compact_list_list.hpp"
#include "istream.hpp"


// Constructors 
template<class T, class Container>
mousse::CompactListList<T, Container>::CompactListList(Istream& is)
{
  operator>>(is, *this);
}


// IOstream Operators 
template<class T, class Container>
mousse::Istream& mousse::operator>>(Istream& is, CompactListList<T, Container>& lst)
{
  is  >> lst.offsets_ >> lst.m_;
  // Note: empty list gets output as two empty lists
  if (lst.offsets_.size() == 0) {
    lst.size_ = 0;
  } else {
    lst.size_ = lst.offsets_.size()-1;
  }
  return is;
}


template<class T, class Container>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CompactListList<T, Container>& lst
)
{
  os << lst.offsets_ << lst.m_;
  return os;
}
