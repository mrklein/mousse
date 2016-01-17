// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sigmoid.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(sigmoid, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  energyScalingFunction,
  sigmoid,
  dictionary
);
// Private Member Functions 
scalar sigmoid::sigmoidScale
  (
    const scalar r,
    const scalar shift,
    const scalar scale
  ) const
{
  return 1.0 / (1.0 + exp( scale * (r - shift)));
}
// Constructors 
sigmoid::sigmoid
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  energyScalingFunction(name, energyScalingFunctionProperties, pairPot),
  sigmoidCoeffs_
  (
    energyScalingFunctionProperties.subDict(typeName + "Coeffs")
  ),
  shift_(readScalar(sigmoidCoeffs_.lookup("shift"))),
  scale_(readScalar(sigmoidCoeffs_.lookup("scale")))
{}
// Member Functions 
void sigmoid::scaleEnergy(scalar& e, const scalar r) const
{
  e *= sigmoidScale(r, shift_, scale_);
}
bool sigmoid::read(const dictionary& energyScalingFunctionProperties)
{
  energyScalingFunction::read(energyScalingFunctionProperties);
  sigmoidCoeffs_ =
    energyScalingFunctionProperties.subDict(typeName + "Coeffs");
  sigmoidCoeffs_.lookup("shift") >> shift_;
  sigmoidCoeffs_.lookup("scale") >> shift_;
  return true;
}
}  // namespace energyScalingFunctions
}  // namespace mousse
