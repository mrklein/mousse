// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nas_edge_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read edgeMesh - .bdf (Bulk Data Format)
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  edgeMesh,
  NASedgeFormat,
  fileExtension,
  bdf
);
// read edgeMesh - .nas (Nastran Data Format)
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  edgeMesh,
  NASedgeFormat,
  fileExtension,
  nas
);

}
}
