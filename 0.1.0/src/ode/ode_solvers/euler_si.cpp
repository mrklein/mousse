// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "euler_si.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(EulerSI, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, EulerSI, dictionary);
}
// Constructors 
mousse::EulerSI::EulerSI(const ODESystem& ode, const dictionary& dict)
:
  ODESolver(ode, dict),
  adaptiveSolver(ode, dict),
  err_(n_),
  dydx_(n_),
  dfdx_(n_),
  dfdy_(n_, n_),
  a_(n_, n_),
  pivotIndices_(n_)
{}
// Member Functions 
mousse::scalar mousse::EulerSI::solve
(
  const scalar x0,
  const scalarField& y0,
  const scalarField& dydx0,
  const scalar dx,
  scalarField& y
) const
{
  odes_.jacobian(x0, y0, dfdx_, dfdy_);
  for (label i=0; i<n_; i++)
  {
    for (label j=0; j<n_; j++)
    {
      a_[i][j] = -dfdy_[i][j];
    }
    a_[i][i] += 1.0/dx;
  }
  LUDecompose(a_, pivotIndices_);
  // Calculate error estimate from the change in state:
  FOR_ALL(err_, i)
  {
    err_[i] = dydx0[i] + dx*dfdx_[i];
  }
  LUBacksubstitute(a_, pivotIndices_, err_);
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + err_[i];
  }
  return normalizeError(y0, y, err_);
}
void mousse::EulerSI::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}
