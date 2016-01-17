// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UDictionary
// Description
//   Template dictionary class which does not manages the storage
//   associated with it.
//   It is derived from DictionaryBase instantiated on a non-memory managed
//   form of intrusive doubly-linked list of \<T\>.
// SourceFiles
//   udictionary.cpp
#ifndef udictionary_hpp_
#define udictionary_hpp_
#include "dictionary_base.hpp"
#include "uidl_list.hpp"
namespace mousse
{
template<class T>
class UDictionary
:
  public DictionaryBase<UIDLList<T>, T>
{
public:
  // Constructors
    //- Null constructor
    UDictionary();
    //- Copy construct
    UDictionary(const UDictionary&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "u_dictionary.cpp"
#endif
#endif
