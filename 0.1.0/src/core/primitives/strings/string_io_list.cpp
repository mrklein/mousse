// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "string_io_list.hpp"
#include "token.hpp"
namespace mousse
{
  DEFINE_COMPOUND_TYPE_NAME(List<string>, stringList);
  ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<string>, stringList);
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(stringIOList, "stringList", 0);
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
  (
    stringListIOList,
    "stringListList",
    0
  );
}
