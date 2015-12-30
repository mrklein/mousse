// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::dampedCoulomb
// Description
// SourceFiles
//   damped_coulomb.cpp
#ifndef damped_coulomb_hpp_
#define damped_coulomb_hpp_
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class dampedCoulomb
:
  public pairPotential
{
  // Private data
    dictionary dampedCoulombCoeffs_;
    scalar alpha_;
public:
  //- Runtime type information
  TypeName("dampedCoulomb");
  // Static data members
    static scalar oneOverFourPiEps0;
  // Constructors
    //- Construct from components
    dampedCoulomb
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~dampedCoulomb()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
