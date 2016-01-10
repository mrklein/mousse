// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef hash_set_cpp_
#define hash_set_cpp_

#include "hash_set.hpp"

// Constructors 
template<class Key, class Hash>
mousse::HashSet<Key, Hash>::HashSet(const UList<Key>& lst)
:
  HashTable<nil, Key, Hash>{2*lst.size()}
{
  FOR_ALL(lst, elemI)
  {
    this->insert(lst[elemI]);
  }
}


template<class Key, class Hash>
template<class AnyType, class AnyHash>
mousse::HashSet<Key, Hash>::HashSet
(
  const HashTable<AnyType, Key, AnyHash>& h
)
:
  HashTable<nil, Key, Hash>{h.size()}
{
  for
  (
    typename HashTable<AnyType, Key, AnyHash>::const_iterator
    cit = h.cbegin();
    cit != h.cend();
    ++cit
  )
  {
    this->insert(cit.key());
  }
}


// Member Functions 
template<class Key, class Hash>
mousse::label mousse::HashSet<Key, Hash>::insert(const UList<Key>& lst)
{
  label count = 0;
  FOR_ALL(lst, elemI)
  {
    if (this->insert(lst[elemI]))
    {
      ++count;
    }
  }
  return count;
}


// Member Operators 
template<class Key, class Hash>
inline bool mousse::HashSet<Key, Hash>::operator[](const Key& key) const
{
  return this->found(key);
}


template<class Key, class Hash>
bool mousse::HashSet<Key, Hash>::operator==(const HashSet<Key, Hash>& rhs) const
{
  // Are all lhs elements in rhs?
  for (const_iterator iter = this->cbegin(); iter != this->cend(); ++iter)
  {
    if (!rhs.found(iter.key()))
    {
      return false;
    }
  }
  // Are all rhs elements in lhs?
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
  {
    if (!this->found(iter.key()))
    {
      return false;
    }
  }
  return true;
}


template<class Key, class Hash>
bool mousse::HashSet<Key, Hash>::operator!=(const HashSet<Key, Hash>& rhs) const
{
  return !(operator==(rhs));
}


template<class Key, class Hash>
void mousse::HashSet<Key, Hash>::operator|=(const HashSet<Key, Hash>& rhs)
{
  // Add rhs elements into lhs
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
  {
    this->insert(iter.key());
  }
}


template<class Key, class Hash>
void mousse::HashSet<Key, Hash>::operator&=(const HashSet<Key, Hash>& rhs)
{
  // Remove elements not also found in rhs
  for (iterator iter = this->begin(); iter != this->end(); ++iter)
  {
    if (!rhs.found(iter.key()))
    {
      this->erase(iter);
    }
  }
}


template<class Key, class Hash>
void mousse::HashSet<Key, Hash>::operator^=(const HashSet<Key, Hash>& rhs)
{
  // Add missed rhs elements, remove duplicate elements
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
  {
    if (this->found(iter.key()))
    {
      this->erase(iter.key());
    }
    else
    {
      this->insert(iter.key());
    }
  }
}


// same as HashTable::erase()
template<class Key, class Hash>
void mousse::HashSet<Key, Hash>::operator-=(const HashSet<Key, Hash>& rhs)
{
  // Remove rhs elements from lhs
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
  {
    this->erase(iter.key());
  }
}


// Global operators
template<class Key, class Hash>
mousse::HashSet<Key, Hash>
mousse::operator|
(
  const HashSet<Key, Hash>& hash1,
  const HashSet<Key, Hash>& hash2
)
{
  HashSet<Key, Hash> out(hash1);
  out |= hash2;
  return out;
}


template<class Key, class Hash>
mousse::HashSet<Key, Hash>
mousse::operator&
(
  const HashSet<Key, Hash>& hash1,
  const HashSet<Key, Hash>& hash2
)
{
  HashSet<Key, Hash> out(hash1);
  out &= hash2;
  return out;
}


template<class Key, class Hash>
mousse::HashSet<Key, Hash>
mousse::operator^
(
  const HashSet<Key, Hash>& hash1,
  const HashSet<Key, Hash>& hash2
)
{
  HashSet<Key, Hash> out(hash1);
  out ^= hash2;
  return out;
}
#endif
