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
// SourceFiles
//   uptr_dictionary.cpp
#ifndef uptr_dictionary_hpp_
#define uptr_dictionary_hpp_
#include "dictionary_base.hpp"
#include "dl_list.hpp"
namespace mousse
{
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
#ifdef NoRepository
#   include "u_ptr_dictionary.cpp"
#endif
#endif
