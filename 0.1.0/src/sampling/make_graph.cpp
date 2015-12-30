// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_graph.hpp"
#include "vol_fields.hpp"
#include "fv_mesh.hpp"
#include "graph.hpp"
namespace mousse
{
void makeGraph
(
  const scalarField& x,
  const volScalarField& vsf,
  const word& graphFormat
)
{
  makeGraph(x, vsf, vsf.name(), graphFormat);
}
void makeGraph
(
  const scalarField& x,
  const volScalarField& vsf,
  const word& name,
  const word& graphFormat
)
{
  fileName path(vsf.rootPath()/vsf.caseName()/"graphs"/vsf.instance());
  mkDir(path);
  makeGraph
  (
    x,
    vsf.internalField(),
    name,
    path,
    graphFormat
  );
}
void makeGraph
(
  const scalarField& x,
  const scalarField& sf,
  const word& name,
  const fileName& path,
  const word& graphFormat
)
{
  graph
  (
    name,
    "x",
    name,
    x,
    sf
  ).write(path/name, graphFormat);
}
}  // namespace mousse
