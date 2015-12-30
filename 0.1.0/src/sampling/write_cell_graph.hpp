// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_cell_graph.cpp
#ifndef write_cell_graph_hpp_
#define write_cell_graph_hpp_
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
