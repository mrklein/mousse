#ifndef CORE_CONTAINERS_DICTIONARIES_UPTR_DICTIONARY_HPP_
#define CORE_CONTAINERS_DICTIONARIES_UPTR_DICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UPtrDictionary
// Description
//   Template dictionary class which does not manages the storage
//   associated with it.
//   It is derived from DictionaryBase instantiated on a non-memory managed
//   form of intrusive doubly-linked list of \<T\>.

#include "dictionary_base.hpp"
#include "dl_list.hpp"


namespace mousse {

template<class T>
class UPtrDictionary
:
  public DictionaryBase<DLList<T*>, T>
{
public:
  // Constructors
    //- Construct given initial table size
    UPtrDictionary(const label size = 128);
    //- Copy construct
    UPtrDictionary(const UPtrDictionary&);
};

}  // namespace mousse

#include "uptr_dictionary.ipp"

#endif
