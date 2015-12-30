// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotentials::pitchForkRing
// Description
// SourceFiles
//   pitch_fork_ring.cpp
#ifndef pitch_fork_ring_hpp_
#define pitch_fork_ring_hpp_
#include "tether_potential.hpp"
namespace mousse
{
namespace tetherPotentials
{
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
  TypeName("pitchForkRing");
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
