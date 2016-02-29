#ifndef CORE_CONTAINERS_LINKED_LISTS_SL_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_SL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SLList
// Description
//   Non-intrusive singly-linked list.
#include "l_list.hpp"
#include "sl_list_base.hpp"
namespace mousse
{
template<class T>
class SLList
:
  public LList<SLListBase, T>
{
public:
  // Constructors
    //- Null construct
    SLList()
    {}
    //- Construct given initial T
    explicit SLList(T a)
    :
      LList<SLListBase, T>(a)
    {}
    //- Construct from Istream
    explicit SLList(Istream& is)
    :
      LList<SLListBase, T>(is)
    {}
};
}  // namespace mousse
#endif
