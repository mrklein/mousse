// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hashed_word_list.hpp"
// Private Member Functions 
void mousse::hashedWordList::rehash()
{
  indices_.clear();
  FOR_ALL(*this, i)
  {
    indices_.insert(List<word>::operator[](i), i);
  }
}
// Constructors 
mousse::hashedWordList::hashedWordList()
:
  List<word>()
{}
mousse::hashedWordList::hashedWordList(const UList<word>& names)
:
  List<word>(names)
{
  rehash();
}
mousse::hashedWordList::hashedWordList(const hashedWordList& names)
:
  List<word>(static_cast<const UList<word>&>(names))
{
  rehash();
}
mousse::hashedWordList::hashedWordList(const Xfer< List<word> >& names)
:
  List<word>(names)
{
  rehash();
}
mousse::hashedWordList::hashedWordList
(
  const label nNames,
  const char** names
)
:
  List<word>(nNames)
{
  FOR_ALL(*this, i)
  {
    List<word>::operator[](i) = names[i];
  }
  rehash();
}
mousse::hashedWordList::hashedWordList
(
  const char** names
)
{
  // count names
  label nNames = 0;
  for (unsigned i = 0; names[i] && *(names[i]); ++i)
  {
    ++nNames;
  }
  List<word>::setSize(nNames);
  FOR_ALL(*this, i)
  {
    List<word>::operator[](i) = names[i];
  }
  rehash();
}
mousse::hashedWordList::hashedWordList(Istream& is)
{
  is  >> *this;
}
// Member Functions 
void mousse::hashedWordList::clear()
{
  List<word>::clear();
  indices_.clear();
}
void mousse::hashedWordList::append(const word& name)
{
  const label idx = size();
  List<word>::append(name);
  indices_.insert(name, idx);
}
void mousse::hashedWordList::transfer(List<word>& lst)
{
  List<word>::transfer(lst);
  rehash();
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, hashedWordList& lst)
{
  is  >> static_cast<List<word>&>(lst);
  lst.rehash();
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const hashedWordList& lst)
{
  os  << static_cast<const List<word>&>(lst);
  return os;
}
