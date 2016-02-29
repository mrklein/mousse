#ifndef SAMPLING_WRITE_CELL_GRAPH_HPP_
#define SAMPLING_WRITE_CELL_GRAPH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_cell_graph.cpp
#include "vol_fields_fwd.hpp"
namespace mousse
{
void writeCellGraph
(
  const volScalarField& vsf,
  const word& graphFormat
);
}  // namespace mousse
#endif
