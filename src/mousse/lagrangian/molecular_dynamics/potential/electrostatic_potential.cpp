// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "electrostatic_potential.hpp"
#include "mathematical_constants.hpp"


// Constructors 
mousse::electrostaticPotential::electrostaticPotential()
:
  prefactor{1.0/(4.0*constant::mathematical::pi*8.854187817e-12)}
{}


// Member Functions 
mousse::scalar mousse::electrostaticPotential::energy(const scalar r) const
{
  return prefactor/r;
}


mousse::scalar mousse::electrostaticPotential::force(const scalar r) const
{
  return prefactor/(r*r);
}

