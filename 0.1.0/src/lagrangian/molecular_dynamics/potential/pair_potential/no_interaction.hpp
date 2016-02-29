#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_NO_INTERACTION_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_NO_INTERACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::noInteraction
// Description
// SourceFiles
//   no_interaction.cpp
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class noInteraction
:
  public pairPotential
{
public:
  //- Runtime type information
  TYPE_NAME("noInteraction");
  // Constructors
    //- Construct from components
    noInteraction
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~noInteraction()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
