// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "starcd_edge_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read edgeMesh
addNamedToRunTimeSelectionTable
(
  edgeMesh,
  STARCDedgeFormat,
  fileExtension,
  inp
);
// write edgeMesh
addNamedToMemberFunctionSelectionTable
(
  edgeMesh,
  STARCDedgeFormat,
  write,
  fileExtension,
  inp
);
}
}
