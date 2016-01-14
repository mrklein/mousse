// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotentials::harmonicSpring
// Description
// SourceFiles
//   harmonic_spring.cpp
#ifndef harmonic_spring_hpp_
#define harmonic_spring_hpp_
#include "tether_potential.hpp"
namespace mousse
{
namespace tetherPotentials
{
class harmonicSpring
:
  public tetherPotential
{
  // Private data
    dictionary harmonicSpringCoeffs_;
    scalar springConstant_;
public:
  //- Runtime type information
  TYPE_NAME("harmonicSpring");
  // Constructors
    //- Construct from components
    harmonicSpring
    (
      const word& name,
      const dictionary& tetherPotentialProperties
    );
  //- Destructor
  ~harmonicSpring()
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
