// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef _ldu_interface_field_ptrs_list_hpp_
#define _ldu_interface_field_ptrs_list_hpp_
#include "_ldu_interface_field.hpp"
#include "uptr_list.hpp"
namespace mousse
{
template<class Type>
class LduInterfaceFieldPtrsList
:
  public UPtrList<const LduInterfaceField<Type> >
{
public:
  LduInterfaceFieldPtrsList(label size)
  :
    UPtrList<const LduInterfaceField<Type> >(size)
  {}
};
}  // namespace mousse
#endif
