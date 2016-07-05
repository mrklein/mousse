// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phi_scheme.hpp"
#include "interface_compression.hpp"


namespace mousse {

MAKE_PHI_SURFACE_INTERPOLATION_SCHEME
(
  interfaceCompression,
  interfaceCompressionLimiter,
  scalar
)

}

