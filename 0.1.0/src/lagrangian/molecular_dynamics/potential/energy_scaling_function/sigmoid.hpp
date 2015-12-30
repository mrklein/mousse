// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunctions::sigmoid
// Description
// SourceFiles
//   sigmoid.cpp
#ifndef sigmoid_hpp_
#define sigmoid_hpp_
#include "energy_scaling_function.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
class sigmoid
:
  public energyScalingFunction
{
  // Private data
    dictionary sigmoidCoeffs_;
    scalar shift_;
    scalar scale_;
  // Private Member Functions
    scalar sigmoidScale
    (
      const scalar r,
      const scalar shift,
      const scalar scale
    ) const;
public:
  //- Runtime type information
  TypeName("sigmoid");
  // Constructors
    //- Construct from components
    sigmoid
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  //- Destructor
  ~sigmoid()
  {}
  // Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& energyScalingFunctionProperties);
};
}  // namespace energyScalingFunctions
}  // namespace mousse
#endif
