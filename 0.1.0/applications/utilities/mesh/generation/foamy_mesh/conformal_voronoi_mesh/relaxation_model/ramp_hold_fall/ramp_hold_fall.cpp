// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ramp_hold_fall.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(rampHoldFall, 0);
addToRunTimeSelectionTable(relaxationModel, rampHoldFall, dictionary);
// Constructors 
rampHoldFall::rampHoldFall
(
  const dictionary& relaxationDict,
  const Time& runTime
)
:
  relaxationModel(typeName, relaxationDict, runTime),
  rampStartRelaxation_(readScalar(coeffDict().lookup("rampStartRelaxation"))),
  holdRelaxation_(readScalar(coeffDict().lookup("holdRelaxation"))),
  fallEndRelaxation_(readScalar(coeffDict().lookup("fallEndRelaxation"))),
  rampEndFraction_(readScalar(coeffDict().lookup("rampEndFraction"))),
  fallStartFraction_(readScalar(coeffDict().lookup("fallStartFraction"))),
  rampGradient_((holdRelaxation_ - rampStartRelaxation_)/(rampEndFraction_)),
  fallGradient_
  (
    (fallEndRelaxation_ - holdRelaxation_)/(1 - fallStartFraction_)
  )
{}
// Member Functions 
scalar rampHoldFall::relaxation()
{
  scalar t = runTime_.time().timeOutputValue();
  scalar tStart = runTime_.time().startTime().value();
  scalar tEnd = runTime_.time().endTime().value();
  scalar tSpan = tEnd - tStart;
  if (tSpan < VSMALL)
  {
    return rampStartRelaxation_;
  }
  if (t - tStart < rampEndFraction_*tSpan)
  {
    // Ramp
    return rampGradient_*((t - tStart)/tSpan) + rampStartRelaxation_;
  }
  else if (t - tStart > fallStartFraction_*tSpan)
  {
    // Fall
    return
      fallGradient_*((t - tStart)/tSpan)
     + fallEndRelaxation_ - fallGradient_;
  }
  else
  {
    //Hold
    return holdRelaxation_;
  }
}
}  // namespace mousse
