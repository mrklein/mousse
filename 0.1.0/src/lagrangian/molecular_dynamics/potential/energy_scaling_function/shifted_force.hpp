// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunctions::shiftedForce
// Description
// SourceFiles
//   shifted_force.cpp
#ifndef shifted_force_hpp_
#define shifted_force_hpp_
#include "energy_scaling_function.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
class shiftedForce
:
  public energyScalingFunction
{
  // Private data
    scalar rCut_;
    scalar e_at_rCut_;
    scalar de_dr_at_rCut_;
public:
  //- Runtime type information
  TypeName("shiftedForce");
  // Constructors
    //- Construct from components
    shiftedForce
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  //- Destructor
  ~shiftedForce()
  {}
  // Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& energyScalingFunctionProperties);
};
}  // namespace energyScalingFunctions
}  // namespace mousse
#endif
