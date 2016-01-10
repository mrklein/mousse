// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "trapezoid.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(Trapezoid, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, Trapezoid, dictionary);
}
// Constructors 
mousse::Trapezoid::Trapezoid(const ODESystem& ode, const dictionary& dict)
:
  ODESolver(ode, dict),
  adaptiveSolver(ode, dict),
  err_(n_)
{}
// Member Functions 
mousse::scalar mousse::Trapezoid::solve
(
  const scalar x0,
  const scalarField& y0,
  const scalarField& dydx0,
  const scalar dx,
  scalarField& y
) const
{
  // Predict the state using 1st-order Trapezoid method
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + dx*dydx0[i];
  }
  // Evaluate the system for the predicted state
  odes_.derivatives(x0 + dx, y, err_);
  // Update the state as the average between the prediction and the correction
  // and estimate the error from the difference
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + 0.5*dx*(dydx0[i] + err_[i]);
    err_[i] = 0.5*dx*(err_[i] - dydx0[i]);
  }
  return normalizeError(y0, y, err_);
}
void mousse::Trapezoid::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}
