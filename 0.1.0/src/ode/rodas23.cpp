// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rodas23.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(rodas23, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, rodas23, dictionary);

const scalar rodas23::c3 = 1;
const scalar rodas23::d1 = 1.0/2.0;
const scalar rodas23::d2 = 3.0/2.0;
const scalar rodas23::a31 = 2;
const scalar rodas23::a41 = 2;
const scalar rodas23::c21 = 4;
const scalar rodas23::c31 = 1;
const scalar rodas23::c32 = -1;
const scalar rodas23::c41 = 1;
const scalar rodas23::c42 = -1;
const scalar rodas23::c43 = -8.0/3.0;
const scalar rodas23::gamma = 1.0/2.0;

}


// Constructors 
mousse::rodas23::rodas23(const ODESystem& ode, const dictionary& dict)
:
  ODESolver{ode, dict},
  adaptiveSolver{ode, dict},
  k1_{n_},
  k2_{n_},
  k3_{n_},
  dy_{n_},
  err_{n_},
  dydx_{n_},
  dfdx_{n_},
  dfdy_{n_, n_},
  a_{n_, n_},
  pivotIndices_{n_}
{}


// Member Functions 
mousse::scalar mousse::rodas23::solve
(
  const scalar x0,
  const scalarField& y0,
  const scalarField& dydx0,
  const scalar dx,
  scalarField& y
) const
{
  odes_.jacobian(x0, y0, dfdx_, dfdy_);
  for (label i=0; i<n_; i++) {
    for (label j=0; j<n_; j++) {
      a_[i][j] = -dfdy_[i][j];
    }
    a_[i][i] += 1.0/(gamma*dx);
  }
  LUDecompose(a_, pivotIndices_);
  // Calculate k1:
  FOR_ALL(k1_, i) {
    k1_[i] = dydx0[i] + dx*d1*dfdx_[i];
  }
  LUBacksubstitute(a_, pivotIndices_, k1_);
  // Calculate k2:
  FOR_ALL(k2_, i) {
    k2_[i] = dydx0[i] + dx*d2*dfdx_[i] + c21*k1_[i]/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k2_);
  // Calculate k3:
  FOR_ALL(y, i) {
    dy_[i] = a31*k1_[i];
    y[i] = y0[i] + dy_[i];
  }
  odes_.derivatives(x0 + dx, y, dydx_);
  FOR_ALL(k3_, i) {
    k3_[i] = dydx_[i] + (c31*k1_[i] + c32*k2_[i])/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k3_);
  // Calculate new state and error
  FOR_ALL(y, i) {
    dy_[i] += k3_[i];
    y[i] = y0[i] + dy_[i];
  }
  odes_.derivatives(x0 + dx, y, dydx_);
  FOR_ALL(err_, i) {
    err_[i] = dydx_[i] + (c41*k1_[i] + c42*k2_[i] + c43*k3_[i])/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, err_);
  FOR_ALL(y, i) {
    y[i] = y0[i] + dy_[i] + err_[i];
  }
  return normalizeError(y0, y, err_);
}


void mousse::rodas23::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}

