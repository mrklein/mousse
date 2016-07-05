// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adaptive_solver.hpp"
#include "add_to_run_time_selection_table.hpp"


// Constructors 
mousse::adaptiveSolver::adaptiveSolver
(
  const ODESystem& ode,
  const dictionary& dict
)
:
  safeScale_{dict.lookupOrDefault<scalar>("safeScale", 0.9)},
  alphaInc_{dict.lookupOrDefault<scalar>("alphaIncrease", 0.2)},
  alphaDec_{dict.lookupOrDefault<scalar>("alphaDecrease", 0.25)},
  minScale_{dict.lookupOrDefault<scalar>("minScale", 0.2)},
  maxScale_{dict.lookupOrDefault<scalar>("maxScale", 10)},
  dydx0_{ode.nEqns()},
  yTemp_{ode.nEqns()}
{}


// Member Functions 
void mousse::adaptiveSolver::solve
(
  const ODESystem& odes,
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  scalar dx = dxTry;
  scalar err = 0.0;
  odes.derivatives(x, y, dydx0_);
  // Loop over solver and adjust step-size as necessary
  // to achieve desired error
  do {
    // Solve step and provide error estimate
    err = solve(x, y, dydx0_, dx, yTemp_);
    // If error is large reduce dx
    if (err > 1) {
      scalar scale = max(safeScale_*pow(err, -alphaDec_), minScale_);
      dx *= scale;
      if (dx < VSMALL) {
        FATAL_ERROR_IN("adaptiveSolver::solve")
          << "stepsize underflow"
          << exit(FatalError);
      }
    }
  } while (err > 1);
  // Update the state
  x += dx;
  y = yTemp_;
  // If the error is small increase the step-size
  if (err > pow(maxScale_/safeScale_, -1.0/alphaInc_)) {
    dxTry = min(max(safeScale_*pow(err, -alphaInc_), minScale_), maxScale_)*dx;
  } else {
    dxTry = safeScale_*maxScale_*dx;
  }
}

