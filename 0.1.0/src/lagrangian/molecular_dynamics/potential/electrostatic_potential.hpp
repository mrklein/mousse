#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ELECTROSTATIC_POTENTIAL_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ELECTROSTATIC_POTENTIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::electrostaticPotential

#include "scalar.hpp"


namespace mousse {

class electrostaticPotential
{
  // Private data
  scalar prefactor;
public:
  // Constructors
    //- Construct and set prefactor
    electrostaticPotential();
  //- Destructor
  ~electrostaticPotential()
  {}
  // Member Functions
    scalar energy(const scalar r) const;
    scalar force(const scalar r) const;
};

}  // namespace mousse

#endif

