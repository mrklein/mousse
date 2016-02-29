#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_RESTRAINED_HARMONIC_SPRING_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_RESTRAINED_HARMONIC_SPRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotentials::restrainedHarmonicSpring
// Description
// SourceFiles
//   restrained_harmonic_spring.cpp
#include "tether_potential.hpp"
namespace mousse
{
namespace tetherPotentials
{
class restrainedHarmonicSpring
:
  public tetherPotential
{
  // Private data
    dictionary restrainedHarmonicSpringCoeffs_;
    scalar springConstant_;
    scalar rR_;
public:
  //- Runtime type information
  TYPE_NAME("restrainedHarmonicSpring");
  // Constructors
    //- Construct from components
    restrainedHarmonicSpring
    (
      const word& name,
      const dictionary& tetherPotentialProperties
    );
  //- Destructor
  ~restrainedHarmonicSpring()
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
