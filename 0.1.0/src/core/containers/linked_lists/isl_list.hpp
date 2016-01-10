// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ISLList
// Description
//   Intrusive singly-linked list.
#ifndef isl_list_hpp_
#define isl_list_hpp_
#include "il_list.hpp"
#include "sl_list_base.hpp"
namespace mousse
{
template<class T>
class ISLList
:
  public ILList<SLListBase, T>
{
public:
  // Constructors
    //- Null construct
    ISLList()
    {}
    //- Construct given initial T
    ISLList(const T& a)
    :
      ILList<SLListBase, T>(a)
    {}
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    ISLList(Istream& is, const INew& inewt)
    :
      ILList<SLListBase, T>(is, inewt)
    {}
    //- Construct from Istream
    ISLList(Istream& is)
    :
      ILList<SLListBase, T>(is)
    {}
};
}  // namespace mousse
#endif
