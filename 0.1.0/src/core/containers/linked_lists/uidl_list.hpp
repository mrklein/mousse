#ifndef CORE_CONTAINERS_LINKED_LISTS_UIDL_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_UIDL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UIDLList
// Description
//   Intrusive doubly-linked list.
#include "uil_list.hpp"
#include "dl_list_base.hpp"
namespace mousse
{
template<class T>
class UIDLList
:
  public UILList<DLListBase, T>
{
public:
  // Constructors
    //- Null construct
    UIDLList()
    {}
    //- Construct given initial T
    UIDLList(T a)
    :
      UILList<DLListBase, T>(a)
    {}
    //- Construct from Istream
    UIDLList(Istream& is)
    :
      UILList<DLListBase, T>(is)
    {}
};
}  // namespace mousse
#endif
