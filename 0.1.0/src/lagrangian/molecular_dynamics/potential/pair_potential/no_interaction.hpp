// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::noInteraction
// Description
// SourceFiles
//   no_interaction.cpp
#ifndef no_interaction_hpp_
#define no_interaction_hpp_
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
  TypeName("noInteraction");
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
