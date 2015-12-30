// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_integration_scheme.hpp"
#include "_euler.hpp"
#include "_analytical.hpp"
#include "scalar.hpp"
#include "vector.hpp"
namespace mousse
{
  makeIntegrationScheme(scalar);
  makeIntegrationSchemeType(Euler, scalar);
  makeIntegrationSchemeType(Analytical, scalar);
  makeIntegrationScheme(vector);
  makeIntegrationSchemeType(Euler, vector);
  makeIntegrationSchemeType(Analytical, vector);
}
