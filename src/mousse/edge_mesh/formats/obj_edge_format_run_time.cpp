// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_edge_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read edgeMesh
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  edgeMesh,
  OBJedgeFormat,
  fileExtension,
  obj
);
// write edgeMesh
ADD_NAMED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  edgeMesh,
  OBJedgeFormat,
  write,
  fileExtension,
  obj
);

}
}
