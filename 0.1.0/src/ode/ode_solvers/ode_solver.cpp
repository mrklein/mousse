// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ode_solver.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(ODESolver, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(ODESolver, dictionary);
}
// Constructors 
mousse::ODESolver::ODESolver(const ODESystem& ode, const dictionary& dict)
:
  odes_(ode),
  n_(ode.nEqns()),
  absTol_(n_, dict.lookupOrDefault<scalar>("absTol", SMALL)),
  relTol_(n_, dict.lookupOrDefault<scalar>("relTol", 1e-4)),
  maxSteps_(10000)
{}
mousse::ODESolver::ODESolver
(
  const ODESystem& ode,
  const scalarField& absTol,
  const scalarField& relTol
)
:
  odes_(ode),
  n_(ode.nEqns()),
  absTol_(absTol),
  relTol_(relTol),
  maxSteps_(10000)
{}
// Member Functions 
mousse::scalar mousse::ODESolver::normalizeError
(
  const scalarField& y0,
  const scalarField& y,
  const scalarField& err
) const
{
  // Calculate the maximum error
  scalar maxErr = 0.0;
  FOR_ALL(err, i)
  {
    scalar tol = absTol_[i] + relTol_[i]*max(mag(y0[i]), mag(y[i]));
    maxErr = max(maxErr, mag(err[i])/tol);
  }
  return maxErr;
}
void mousse::ODESolver::solve
(
  scalar& x,
  scalarField& y,
  stepState& step
) const
{
  scalar x0 = x;
  solve(x, y, step.dxTry);
  step.dxDid = x - x0;
}
void mousse::ODESolver::solve
(
  const scalar xStart,
  const scalar xEnd,
  scalarField& y,
  scalar& dxTry
) const
{
  stepState step(dxTry);
  scalar x = xStart;
  for (label nStep=0; nStep<maxSteps_; nStep++)
  {
    // Store previous iteration dxTry
    scalar dxTry0 = step.dxTry;
    step.reject = false;
    // Check if this is a truncated step and set dxTry to integrate to xEnd
    if ((x + step.dxTry - xEnd)*(x + step.dxTry - xStart) > 0)
    {
      step.last = true;
      step.dxTry = xEnd - x;
    }
    // Integrate as far as possible up to step.dxTry
    solve(x, y, step);
    // Check if reached xEnd
    if ((x - xEnd)*(xEnd - xStart) >= 0)
    {
      if (nStep > 0 && step.last)
      {
        step.dxTry = dxTry0;
      }
      dxTry = step.dxTry;
      return;
    }
    step.first = false;
    // If the step.dxTry was reject set step.prevReject
    if (step.reject)
    {
      step.prevReject = true;
    }
  }
  FATAL_ERROR_IN
  (
    "ODESolver::solve"
    "(const scalar xStart, const scalar xEnd,"
    "scalarField& y, scalar& dxTry) const"
  )   << "Integration steps greater than maximum " << maxSteps_
    << "xStart = " << xStart << ", xEnd = " << xEnd
    << ", x = " << x << ", dxDid = " << step.dxDid
    << exit(FatalError);
}
