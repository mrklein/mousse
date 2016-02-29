#ifndef RANDOM_PROCESSES_K_SHELL_INTEGRATION_HPP_
#define RANDOM_PROCESSES_K_SHELL_INTEGRATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
