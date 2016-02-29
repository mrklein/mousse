#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_COULOMB_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_COULOMB_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::coulomb
// Description
// SourceFiles
//   coulomb.cpp
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class coulomb
:
  public pairPotential
{
public:
  //- Runtime type information
  TYPE_NAME("coulomb");
  // Static data members
    static scalar oneOverFourPiEps0;
  // Constructors
    //- Construct from components
    coulomb
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~coulomb()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
