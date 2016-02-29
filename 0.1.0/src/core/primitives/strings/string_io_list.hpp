#ifndef CORE_PRIMITIVES_STRINGS_STRING_IO_LIST_HPP_
#define CORE_PRIMITIVES_STRINGS_STRING_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "string_list.hpp"
#include "iolist.hpp"
namespace mousse
{
  typedef IOList<string> stringIOList;
  typedef IOList<stringList> stringListIOList;
}
#endif
