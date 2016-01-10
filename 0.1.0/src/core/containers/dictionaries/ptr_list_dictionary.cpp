// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptr_list_dictionary.hpp"

// Constructors 
template<class T>
mousse::PtrListDictionary<T>::PtrListDictionary(const label size)
:
  DictionaryBase<PtrList<T>, T>(2*size)
{
  PtrList<T>::setSize(size);
}


template<class T>
mousse::PtrListDictionary<T>::PtrListDictionary(const PtrListDictionary& dict)
:
  DictionaryBase<PtrList<T>, T>(dict)
{}


template<class T>
template<class INew>
mousse::PtrListDictionary<T>::PtrListDictionary(Istream& is, const INew& iNew)
:
  DictionaryBase<PtrList<T>, T>(is, iNew)
{}


template<class T>
mousse::PtrListDictionary<T>::PtrListDictionary(Istream& is)
:
  DictionaryBase<PtrList<T>, T>(is)
{}


// Member Functions 
template<class T>
inline mousse::autoPtr<T> mousse::PtrListDictionary<T>::set
(
  const label i,
  const word& key,
  T* ptr
)
{
  if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
  {
    FATAL_ERROR_IN
    (
      "PtrListDictionary<T>::set(const label i, const word& key, T* ptr)"
    )
    << "Cannot insert with key '" << key << "' into hash-table"
    << abort(FatalError);
  }
  return PtrList<T>::set(i, ptr);
}


template<class T>
inline mousse::autoPtr<T> mousse::PtrListDictionary<T>::set
(
  const label i,
  const word& key,
  autoPtr<T>& aptr
)
{
  T* ptr = aptr.ptr();
  if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
  {
    FATAL_ERROR_IN
    (
      "PtrListDictionary<T>::"
      "set(const label i, const word& key, autoPtr<T>& aptr)"
    )
    << "Cannot insert with key '" << key << "' into hash-table"
    << abort(FatalError);
  }

  return PtrList<T>::set(i, ptr);
}


template<class T>
inline mousse::autoPtr<T> mousse::PtrListDictionary<T>::set
(
  const label i,
  const word& key,
  tmp<T>& t
)
{
  T* ptr = t.ptr();
  if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
  {
    FATAL_ERROR_IN
    (
      "PtrListDictionary<T>::"
      "set(const label i, const word& key, tmp<T>& t)"
    )
    << "Cannot insert with key '" << key << "' into hash-table"
    << abort(FatalError);
  }
  return PtrList<T>::set(i, ptr);
}
