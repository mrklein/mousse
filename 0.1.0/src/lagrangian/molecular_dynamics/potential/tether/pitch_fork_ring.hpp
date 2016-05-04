#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_PITCH_FORK_RING_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_PITCH_FORK_RING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotentials::pitchForkRing

#include "tether_potential.hpp"


namespace mousse {
namespace tetherPotentials {

class pitchForkRing
:
  public tetherPotential
{
  // Private data
    dictionary pitchForkRingCoeffs_;
    scalar mu_;
    scalar alpha_;
    scalar rOrbit_;
public:
  //- Runtime type information
  TYPE_NAME("pitchForkRing");
  // Constructors
    //- Construct from components
    pitchForkRing
    (
      const word& name,
      const dictionary& tetherPotentialProperties
    );
  //- Destructor
  ~pitchForkRing()
  {}
  // Member Functions
    scalar energy(const vector r) const;
    vector force(const vector r) const;
    //- Read dictionary
    bool read(const dictionary& tetherPotentialProperties);
};

}  // namespace tetherPotentials
}  // namespace mousse

#endif

