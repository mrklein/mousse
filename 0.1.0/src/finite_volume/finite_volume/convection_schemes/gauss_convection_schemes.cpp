// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_convection_scheme.hpp"
#include "fv_mesh.hpp"

int mousse::fv::warnUnboundedGauss
(
  mousse::debug::debugSwitch("warnUnboundedGauss", true)
);

MAKE_FV_CONVECTION_SCHEME(gaussConvectionScheme)
