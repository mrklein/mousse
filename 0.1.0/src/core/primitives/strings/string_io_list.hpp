// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef string_io_list_hpp_
#define string_io_list_hpp_
#include "string_list.hpp"
#include "iolist.hpp"
namespace mousse
{
  typedef IOList<string> stringIOList;
  typedef IOList<stringList> stringListIOList;
}
#endif
