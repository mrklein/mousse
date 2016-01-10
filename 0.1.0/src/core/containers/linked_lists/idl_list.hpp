// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IDLList
// Description
//   Intrusive doubly-linked list.
#ifndef idl_list_hpp_
#define idl_list_hpp_
#include "il_list.hpp"
#include "dl_list_base.hpp"
namespace mousse
{
template<class T>
class IDLList
:
  public ILList<DLListBase, T>
{
public:
  // Constructors
    //- Null construct
    IDLList()
    {}
    //- Construct given initial T
    IDLList(const T& a)
    :
      ILList<DLListBase, T>(a)
    {}
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    IDLList(Istream& is, const INew& inewt)
    :
      ILList<DLListBase, T>(is, inewt)
    {}
    //- Construct from Istream
    IDLList(Istream& is)
    :
      ILList<DLListBase, T>(is)
    {}
    //- Copy constructor with additional argument for clone
    template<class CloneArg>
    IDLList(const IDLList<T>& idll, const CloneArg& cloneArg)
    :
      ILList<DLListBase, T>(idll, cloneArg)
    {}
};
}  // namespace mousse
#endif
