// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef ldu_interface_field_ptrs_list_hpp_
#define ldu_interface_field_ptrs_list_hpp_
#include "ldu_interface_field.hpp"
#include "uptr_list.hpp"
namespace mousse
{
  typedef UPtrList<const lduInterfaceField> lduInterfaceFieldPtrsList;
}
#endif
