// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_edge_mesh_format.hpp"
#include "extended_edge_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read extendedEdgeMesh
addNamedToRunTimeSelectionTable
(
  extendedEdgeMesh,
  extendedEdgeMeshFormat,
  fileExtension,
  extendedFeatureEdgeMesh         // extension
);
}
}
