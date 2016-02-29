#ifndef CORE_CONTAINERS_LINKED_LISTS_SL_PTR_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_SL_PTR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SLPtrList
// Description
//   Non-intrusive singly-linked pointer list.
#include "l_ptr_list.hpp"
#include "sl_list_base.hpp"
namespace mousse
{
template<class T>
class SLPtrList
:
  public LPtrList<SLListBase, T>
{
public:
  // Constructors
    //- Null construct
    SLPtrList()
    {}
    //- Construct given initial T*
    SLPtrList(T* a)
    :
      LPtrList<SLListBase, T>(a)
    {}
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    SLPtrList(Istream& is, const INew& inewt)
    :
      LPtrList<SLListBase, T>(is, inewt)
    {}
    //- Construct from Istream
    SLPtrList(Istream& is)
    :
      LPtrList<SLListBase, T>(is)
    {}
};
}  // namespace mousse
#endif
