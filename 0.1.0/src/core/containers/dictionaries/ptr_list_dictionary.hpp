#ifndef CORE_CONTAINERS_DICTIONARIES_PTR_LIST_DICTIONARY_HPP_
#define CORE_CONTAINERS_DICTIONARIES_PTR_LIST_DICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PtrListDictionary
// Description
//   Template dictionary class which manages the storage associated with it.
//   It is derived from DictionaryBase instantiated on the memory managed PtrList
//   of \<T\> to provide ordered indexing in addition to the dictionary lookup.
// SourceFiles
//   ptr_list_dictionary.cpp


#include "dictionary_base.hpp"
#include "ptr_list.hpp"

namespace mousse
{

template<class T>
class PtrListDictionary
:
  public DictionaryBase<PtrList<T>, T>
{
public:
  // Constructors
    //- Construct given initial list size
    PtrListDictionary(const label size);

    //- Copy construct
    PtrListDictionary(const PtrListDictionary&);

    //- Construct from Istream using given Istream constructor class
    template<class INew>
    PtrListDictionary(Istream&, const INew&);

    //- Construct from Istream
    PtrListDictionary(Istream&);

  // Member functions
    //- Set element to pointer provided and return old element
    autoPtr<T> set(const label, const word& key, T*);

    //- Set element to autoPtr value provided and return old element
    autoPtr<T> set(const label, const word& key, autoPtr<T>&);

    //- Set element to tmp value provided and return old element
    autoPtr<T> set(const label, const word& key, tmp<T>&);

  // Member operators
    using PtrList<T>::operator[];

    //- Find and return entry
    const T& operator[](const word& key) const
    {
      return *DictionaryBase<PtrList<T>, T>::operator[](key);
    }

    //- Find and return entry
    T& operator[](const word& key)
    {
      return *DictionaryBase<PtrList<T>, T>::operator[](key);
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "ptr_list_dictionary.cpp"
#endif
#endif
