#ifndef CORE_CONTAINERS_LINKED_LISTS_DL_PTR_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_DL_PTR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DLPtrList
// Description
//   Non-intrusive doubly-linked pointer list.

#include "l_ptr_list.hpp"
#include "dl_list_base.hpp"


namespace mousse {

template<class T>
class DLPtrList
:
  public LPtrList<DLListBase, T>
{
public:

  // Constructors

    //- Null construct
    DLPtrList()
    {}

    //- Construct from Istream using given Istream constructor class
    template<class INew>
    DLPtrList(Istream& is, const INew& inewt)
    :
      LPtrList<DLListBase, T>{is, inewt}
    {}

    //- Construct from Istream
    DLPtrList(Istream& is)
    :
      LPtrList<DLListBase, T>{is}
    {}
};

}  // namespace mousse

#endif
