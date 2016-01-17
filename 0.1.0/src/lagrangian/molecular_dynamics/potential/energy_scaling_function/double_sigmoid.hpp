// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunctions::doubleSigmoid
// Description
// SourceFiles
//   double_sigmoid.cpp
#ifndef double_sigmoid_hpp_
#define double_sigmoid_hpp_
#include "energy_scaling_function.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
class doubleSigmoid
:
  public energyScalingFunction
{
  // Private data
    dictionary doubleSigmoidCoeffs_;
    scalar shift1_;
    scalar scale1_;
    scalar shift2_;
    scalar scale2_;
  // Private Member Functions
    scalar sigmoidScale
    (
      const scalar r,
      const scalar shift,
      const scalar scale
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("doubleSigmoid");
  // Constructors
    //- Construct from components
    doubleSigmoid
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  //- Destructor
  ~doubleSigmoid()
  {}
  // Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& energyScalingFunctionProperties);
};
}  // namespace energyScalingFunctions
}  // namespace mousse
#endif
