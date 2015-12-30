// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   make_graph.cpp
#ifndef make_graph_hpp_
#define make_graph_hpp_
#include "primitive_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
class fileName;
void makeGraph
(
  const scalarField& x,
  const volScalarField& vsf,
  const word& graphFormat
);
void makeGraph
(
  const scalarField& x,
  const volScalarField& vsf,
  const word& name,
  const word& graphFormat
);
void makeGraph
(
  const scalarField& x,
  const scalarField& sf,
  const word& name,
  const fileName& path,
  const word& graphFormat
);
}  // namespace mousse
#endif
