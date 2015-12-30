// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_patch_graph.cpp
#ifndef write_patch_graph_hpp_
#define write_patch_graph_hpp_
#include "vol_fields_fwd.hpp"
namespace mousse
{
void writePatchGraph
(
  const volScalarField& vsf,
  const label patchLabel,
  const direction d,
  const word& graphFormat
);
}  // namespace mousse
#endif
