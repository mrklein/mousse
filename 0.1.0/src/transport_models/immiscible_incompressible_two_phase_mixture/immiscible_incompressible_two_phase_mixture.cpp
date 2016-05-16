// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "immiscible_incompressible_two_phase_mixture.hpp"


// Constructors 
mousse::immiscibleIncompressibleTwoPhaseMixture::
immiscibleIncompressibleTwoPhaseMixture
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  incompressibleTwoPhaseMixture{U, phi},
  interfaceProperties{alpha1(), U, *this}
{}
