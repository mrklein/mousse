#ifndef CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_LDU_INTERFACE_PTRS_LIST_HPP_
#define CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_LDU_INTERFACE_PTRS_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_interface.hpp"
#include "uptr_list.hpp"
namespace mousse
{
  typedef UPtrList<const lduInterface> lduInterfacePtrsList;
}
#endif
