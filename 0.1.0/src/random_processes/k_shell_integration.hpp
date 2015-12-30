// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef k_shell_integration_hpp_
#define k_shell_integration_hpp_
#include "complex_fields.hpp"
#include "kmesh.hpp"
#include "graph.hpp"
namespace mousse
{
graph kShellIntegration
(
  const complexVectorField& Ek,
  const Kmesh& K
);
graph kShellMean
(
  const complexVectorField& Ek,
  const Kmesh& K
);
}  // namespace mousse
#endif
