#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_EXPONENTIAL_REPULSION_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_EXPONENTIAL_REPULSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::exponentialRepulsion
// Description
// SourceFiles
//   exponential_repulsion.cpp
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class exponentialRepulsion
:
  public pairPotential
{
  // Private data
    dictionary exponentialRepulsionCoeffs_;
    scalar rm_;
    scalar epsilon_;
public:
  //- Runtime type information
  TYPE_NAME("exponentialRepulsion");
  // Constructors
    //- Construct from components
    exponentialRepulsion
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~exponentialRepulsion()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
