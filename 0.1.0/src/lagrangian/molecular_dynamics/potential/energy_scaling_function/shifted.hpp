#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ENERGY_SCALING_FUNCTION_SHIFTED_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ENERGY_SCALING_FUNCTION_SHIFTED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunctions::shifted
// Description
// SourceFiles
//   shifted.cpp
#include "energy_scaling_function.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
class shifted
:
  public energyScalingFunction
{
  // Private data
    scalar e_at_rCut_;
public:
  //- Runtime type information
  TYPE_NAME("shifted");
  // Constructors
    //- Construct from components
    shifted
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  //- Destructor
  ~shifted()
  {}
  // Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& energyScalingFunctionProperties);
};
}  // namespace energyScalingFunctions
}  // namespace mousse
#endif
