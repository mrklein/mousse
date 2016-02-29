#ifndef CORE_CONTAINERS_DICTIONARIES_TDICTIONARY_HPP_
#define CORE_CONTAINERS_DICTIONARIES_TDICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Dictionary
// Description
//   Gerneral purpose template dictionary class which manages the storage
//   associated with it.
//   It is derived from DictionaryBase instantiated on a memory managed form
//   of intrusive doubly-linked list of \<T\>.
// SourceFiles
//   _dictionary.cpp
#include "dictionary_base.hpp"
#include "idl_list.hpp"
namespace mousse
{
template<class T>
class Dictionary
:
  public DictionaryBase<IDLList<T>, T>
{
public:
  // Constructors
    //- Construct given initial table size
    Dictionary(const label size = 128);
    //- Copy construct
    Dictionary(const Dictionary&);
  // Member functions
    //- Remove an entry specified by keyword and delete the pointer.
    //  Returns true if the keyword was found
    bool erase(const word& keyword);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_dictionary.cpp"
#endif
#endif
