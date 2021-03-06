// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "euler.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(Euler, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, Euler, dictionary);

}


// Constructors 
mousse::Euler::Euler(const ODESystem& ode, const dictionary& dict)
:
  ODESolver{ode, dict},
  adaptiveSolver{ode, dict},
  err_{n_}
{}


// Member Functions 
mousse::scalar mousse::Euler::solve
(
  const scalar /*x0*/,
  const scalarField& y0,
  const scalarField& dydx0,
  const scalar dx,
  scalarField& y
) const
{
  // Calculate error estimate from the change in state:
  FOR_ALL(err_, i) {
    err_[i] = dx*dydx0[i];
  }
  // Update the state
  FOR_ALL(y, i) {
    y[i] = y0[i] + err_[i];
  }
  return normalizeError(y0, y, err_);
}


void mousse::Euler::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}

