// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "immiscible_incompressible_three_phase_mixture.hpp"


// Constructors 
mousse::immiscibleIncompressibleThreePhaseMixture::
immiscibleIncompressibleThreePhaseMixture
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  incompressibleThreePhaseMixture{U, phi},
  threePhaseInterfaceProperties
  {
    static_cast<incompressibleThreePhaseMixture&>(*this)
  }
{}

