// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "piecewise_linear_ramp.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(piecewiseLinearRamp, 0);
addToRunTimeSelectionTable
(
  faceAreaWeightModel,
  piecewiseLinearRamp,
  dictionary
);
// Constructors 
piecewiseLinearRamp::piecewiseLinearRamp
(
  const dictionary& faceAreaWeightDict
)
:
  faceAreaWeightModel(typeName, faceAreaWeightDict),
  lAF_(readScalar(coeffDict().lookup("lowerAreaFraction"))),
  uAF_(readScalar(coeffDict().lookup("upperAreaFraction")))
{}
// Member Functions 
scalar piecewiseLinearRamp::faceAreaWeight(scalar faceAreaFraction) const
{
  if (faceAreaFraction < lAF_)
  {
    return 0;
  }
  else if (faceAreaFraction < uAF_)
  {
    return faceAreaFraction/((uAF_ - lAF_)) - lAF_/(uAF_ - lAF_);
  }
  else
  {
    return 1;
  }
}
}  // namespace mousse
