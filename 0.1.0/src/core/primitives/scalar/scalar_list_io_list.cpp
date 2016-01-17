// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_list_io_list.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse
{
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
  (
    scalarListIOList,
    "scalarListList",
    0
  );

  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
  (
    scalarListCompactIOList,
    "scalarListCompactList",
    0
  );
}
