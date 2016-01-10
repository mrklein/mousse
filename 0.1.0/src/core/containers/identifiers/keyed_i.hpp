// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iostreams.hpp"

// Constructors 
template<class T>
inline mousse::Keyed<T>::Keyed()
:
  key_(-1)
{}
template<class T>
inline mousse::Keyed<T>::Keyed(const T& item, const label key)
:
  T(item),
  key_(key)
{}
template<class T>
inline mousse::Keyed<T>::Keyed(const Xfer<T>& item, const label key)
:
  T(item),
  key_(key)
{}
template<class T>
inline mousse::Keyed<T>::Keyed(Istream& is)
{
  is >> *this;
}
// Member Functions 
template<class T>
inline mousse::label mousse::Keyed<T>::key() const
{
  return key_;
}
template<class T>
inline mousse::label& mousse::Keyed<T>::key()
{
  return key_;
}
template<class T>
inline mousse::List<mousse::Keyed<T> >
mousse::Keyed<T>::createList(const List<T>& lst, const label key)
{
  List<Keyed<T> > newList(lst.size());
  FOR_ALL(lst, elemI)
  {
    newList[elemI] = Keyed(lst[elemI], key);
  }
  return newList;
}
template<class T>
inline mousse::List<mousse::Keyed<T> >
mousse::Keyed<T>::createList(const List<T>& lst, const labelUList& keys)
{
  if (lst.size() != keys.size())
  {
    FATAL_ERROR_IN
    (
      "mousse::Keyed<T>::createList(const List<T>&, const labelUList&)"
    )
    << "size mismatch adding keys to a list:" << nl
    << "List has size " << lst.size()
    << " and keys has size " << keys.size() << nl
    << abort(FatalError);
  }
  List<Keyed<T> > newList(lst.size());
  FOR_ALL(lst, elemI)
  {
    newList[elemI] = Keyed(lst[elemI], keys[elemI]);
  }
  return newList;
}
// Ostream Operator 
template<class T>
inline mousse::Istream& mousse::operator>>(Istream& is, Keyed<T>& item)
{
  // Read beginning of Keyed item/key pair
  is.readBegin("Keyed<T>");
  is >> static_cast<T&>(item);
  is >> item.key_;
  // Read end of Keyed item/key pair
  is.readEnd("Keyed<T>");
  // Check state of Ostream
  is.check("Istream& operator>>(Istream&, Keyed&)");
  return is;
}
template<class T>
inline mousse::Ostream& mousse::operator<<(Ostream& os, const Keyed<T>& item)
{
  os  << token::BEGIN_LIST
    << static_cast<const T&>(item)
    << token::SPACE << item.key_
    << token::END_LIST;
  return os;
}
