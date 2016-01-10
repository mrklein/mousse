// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_feature_edge_mesh_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read extendedEdgeMesh
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  edgeMesh,
  extendedFeatureEdgeMeshFormat,
  fileExtension,
  featureEdgeMesh
);
}
}
