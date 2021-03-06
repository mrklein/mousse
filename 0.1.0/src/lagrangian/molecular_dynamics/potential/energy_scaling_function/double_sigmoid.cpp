// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "double_sigmoid.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace energyScalingFunctions {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(doubleSigmoid, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  energyScalingFunction,
  doubleSigmoid,
  dictionary
);


// Private Member Functions 
scalar doubleSigmoid::sigmoidScale
(
  const scalar r,
  const scalar shift,
  const scalar scale
) const
{
  return 1.0 / (1.0 + exp( scale * (r - shift)));
}


// Constructors 
doubleSigmoid::doubleSigmoid
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  energyScalingFunction{name, energyScalingFunctionProperties, pairPot},
  doubleSigmoidCoeffs_
  {
    energyScalingFunctionProperties.subDict(typeName + "Coeffs")
  },
  shift1_{readScalar(doubleSigmoidCoeffs_.lookup("shift1"))},
  scale1_{readScalar(doubleSigmoidCoeffs_.lookup("scale1"))},
  shift2_{readScalar(doubleSigmoidCoeffs_.lookup("shift2"))},
  scale2_{readScalar(doubleSigmoidCoeffs_.lookup("scale2"))}
{}


// Member Functions 
void doubleSigmoid::scaleEnergy(scalar& e, const scalar r) const
{
  e *= sigmoidScale(r, shift1_, scale1_) * sigmoidScale(r, shift2_, scale2_);
}


bool doubleSigmoid::read(const dictionary& energyScalingFunctionProperties)
{
  energyScalingFunction::read(energyScalingFunctionProperties);
  doubleSigmoidCoeffs_ =
    energyScalingFunctionProperties.subDict(typeName + "Coeffs");
  doubleSigmoidCoeffs_.lookup("shift1") >> shift1_;
  doubleSigmoidCoeffs_.lookup("scale1") >> scale1_;
  doubleSigmoidCoeffs_.lookup("shift2") >> shift2_;
  doubleSigmoidCoeffs_.lookup("scale2") >> scale2_;
  return true;
}

}  // namespace energyScalingFunctions
}  // namespace mousse

