// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "hash_ptr_table.hpp"


// Constructors 
// Construct given initial table size
template<class T, class Key, class Hash>
mousse::HashPtrTable<T, Key, Hash>::HashPtrTable(const label size)
:
  HashTable<T*, Key, Hash>{size}
{}


// Construct as copy
template<class T, class Key, class Hash>
mousse::HashPtrTable<T, Key, Hash>::HashPtrTable
(
  const HashPtrTable<T, Key, Hash>& ht
)
:
  HashTable<T*, Key, Hash>{}
{
  for (const_iterator iter = ht.begin(); iter != ht.end(); ++iter) {
    this->insert(iter.key(), new T(**iter));
  }
}


// Destructor 
template<class T, class Key, class Hash>
mousse::HashPtrTable<T, Key, Hash>::~HashPtrTable()
{
  clear();
}


// Member Functions 
template<class T, class Key, class Hash>
T* mousse::HashPtrTable<T, Key, Hash>::remove(iterator& it)
{
  T* elemPtr = *it;
  HashTable<T*, Key, Hash>::erase(it);
  return elemPtr;
}


template<class T, class Key, class Hash>
bool mousse::HashPtrTable<T, Key, Hash>::erase(iterator& it)
{
  T* elemPtr = *it;
  if (HashTable<T*, Key, Hash>::erase(it)) {
    if (elemPtr) {
      delete elemPtr;
    }
    return true;
  } else {
    return false;
  }
}


template<class T, class Key, class Hash>
void mousse::HashPtrTable<T, Key, Hash>::clear()
{
  for (iterator iter = this->begin(); iter != this->end(); ++iter) {
    delete *iter;
  }
  HashTable<T*, Key, Hash>::clear();
}


// Member Operators 
template<class T, class Key, class Hash>
void mousse::HashPtrTable<T, Key, Hash>::operator=
(
  const HashPtrTable<T, Key, Hash>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs) {
    FATAL_ERROR_IN
    (
      "HashPtrTable<T, Key, Hash>::operator="
      "(const HashPtrTable<T, Key, Hash>&)"
    )
    << "attempted assignment to self"
    << abort(FatalError);
  }
  this->clear();
  for (const_iterator iter = rhs.begin(); iter != rhs.end(); ++iter) {
    this->insert(iter.key(), new T(**iter));
  }
}

// Friend Operators 
#include "hash_ptr_table_io.ipp"
