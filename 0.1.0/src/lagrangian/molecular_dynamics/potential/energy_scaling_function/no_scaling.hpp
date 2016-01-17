// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunctions::noScaling
// Description
// SourceFiles
//   no_scaling.cpp
#ifndef no_scaling_hpp_
#define no_scaling_hpp_
#include "energy_scaling_function.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
class noScaling
:
  public energyScalingFunction
{
public:
  //- Runtime type information
  TYPE_NAME("noScaling");
  // Constructors
    //- Construct from components
    noScaling
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  //- Destructor
  ~noScaling()
  {}
  // Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& energyScalingFunctionProperties);
};
}  // namespace energyScalingFunctions
}  // namespace mousse
#endif
