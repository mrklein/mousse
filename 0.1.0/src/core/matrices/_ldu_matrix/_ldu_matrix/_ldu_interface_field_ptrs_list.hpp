#ifndef CORE_MATRICES_TLDU_MATRIX_TLDU_MATRIX_TLDU_INTERFACE_FIELD_PTRS_LIST_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TLDU_MATRIX_TLDU_INTERFACE_FIELD_PTRS_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_ldu_interface_field.hpp"
#include "uptr_list.hpp"


namespace mousse {

template<class Type>
class LduInterfaceFieldPtrsList
:
  public UPtrList<const LduInterfaceField<Type>>
{
public:
  LduInterfaceFieldPtrsList(label size)
  :
    UPtrList<const LduInterfaceField<Type>>{size}
  {}
};

}  // namespace mousse

#endif
