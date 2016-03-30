#ifndef CORE_CONTAINERS_LINKED_LISTS_DL_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_DL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DLList
// Description
//   Non-intrusive doubly-linked list.

#include "l_list.hpp"
#include "dl_list_base.hpp"


namespace mousse {

template<class T>
class DLList
:
  public LList<DLListBase, T>
{
public:
  // Constructors
    //- Null construct
    DLList()
    {}
    //- Construct given initial T
    DLList(T a)
    :
      LList<DLListBase, T>{a}
    {}
    //- Construct from Istream
    DLList(Istream& is)
    :
      LList<DLListBase, T>{is}
    {}
};

}  // namespace mousse

#endif
