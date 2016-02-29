#ifndef CORE_CONTAINERS_DICTIONARIES_PTR_DICTIONARY_HPP_
#define CORE_CONTAINERS_DICTIONARIES_PTR_DICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PtrDictionary
// Description
//   Template dictionary class which manages the storage associated with it.
//   It is derived from DictionaryBase instantiated on a memory managed form of
//   intrusive doubly-linked list of \<T\>.
// SourceFiles
//   ptr_dictionary.cpp
#include "dictionary_base.hpp"
#include "dl_ptr_list.hpp"
namespace mousse
{
template<class T>
class PtrDictionary
:
  public DictionaryBase<DLPtrList<T>, T>
{
public:
  // Constructors
    //- Construct given initial table size
    PtrDictionary(const label size = 128);
    //- Copy construct
    PtrDictionary(const PtrDictionary&);
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    PtrDictionary(Istream&, const INew&);
    //- Construct from Istream
    PtrDictionary(Istream&);
  // Member operators
    //- Find and return entry
    const T& operator[](const word& key) const
    {
      return *DictionaryBase<DLPtrList<T>, T>::operator[](key);
    }
    //- Find and return entry
    T& operator[](const word& key)
    {
      return *DictionaryBase<DLPtrList<T>, T>::operator[](key);
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "ptr_dictionary.cpp"
#endif
#endif
