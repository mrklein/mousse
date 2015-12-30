// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::lennardJones
// Description
// SourceFiles
//   lennard_jones.cpp
#ifndef lennard_jones_hpp_
#define lennard_jones_hpp_
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class lennardJones
:
  public pairPotential
{
  // Private data
    dictionary lennardJonesCoeffs_;
    scalar sigma_;
    scalar epsilon_;
public:
  //- Runtime type information
  TypeName("lennardJones");
  // Constructors
    //- Construct from components
    lennardJones
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~lennardJones()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
