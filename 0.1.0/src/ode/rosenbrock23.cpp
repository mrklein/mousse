// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rosenbrock23.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(Rosenbrock23, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, Rosenbrock23, dictionary);

const scalar Rosenbrock23::a21 = 1;
const scalar Rosenbrock23::a31 = 1;
const scalar Rosenbrock23::a32 = 0;
const scalar Rosenbrock23::c21 = -1.0156171083877702091975600115545;
const scalar Rosenbrock23::c31 = 4.0759956452537699824805835358067;
const scalar Rosenbrock23::c32 = 9.2076794298330791242156818474003;
const scalar Rosenbrock23::b1 = 1;
const scalar Rosenbrock23::b2 = 6.1697947043828245592553615689730;
const scalar Rosenbrock23::b3 = -0.4277225654321857332623837380651;
const scalar Rosenbrock23::e1 = 0.5;
const scalar Rosenbrock23::e2 = -2.9079558716805469821718236208017;
const scalar Rosenbrock23::e3 = 0.2235406989781156962736090927619;
const scalar Rosenbrock23::gamma = 0.43586652150845899941601945119356;
const scalar Rosenbrock23::c2 = 0.43586652150845899941601945119356;
const scalar Rosenbrock23::d1 = 0.43586652150845899941601945119356;
const scalar Rosenbrock23::d2 = 0.24291996454816804366592249683314;
const scalar Rosenbrock23::d3 = 2.1851380027664058511513169485832;

}


// Constructors 
mousse::Rosenbrock23::Rosenbrock23(const ODESystem& ode, const dictionary& dict)
:
  ODESolver{ode, dict},
  adaptiveSolver{ode, dict},
  k1_{n_},
  k2_{n_},
  k3_{n_},
  err_{n_},
  dydx_{n_},
  dfdx_{n_},
  dfdy_{n_, n_},
  a_{n_, n_},
  pivotIndices_{n_}
{}


// Member Functions 
mousse::scalar mousse::Rosenbrock23::solve
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
  FOR_ALL(y, i) {
    y[i] = y0[i] + a21*k1_[i];
  }
  odes_.derivatives(x0 + c2*dx, y, dydx_);
  FOR_ALL(k2_, i) {
    k2_[i] = dydx_[i] + dx*d2*dfdx_[i] + c21*k1_[i]/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k2_);
  // Calculate k3:
  FOR_ALL(k3_, i) {
    k3_[i] = dydx_[i] + dx*d3*dfdx_[i] + (c31*k1_[i] + c32*k2_[i])/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k3_);
  // Calculate error and update state:
  FOR_ALL(y, i) {
    y[i] = y0[i] + b1*k1_[i] + b2*k2_[i] + b3*k3_[i];
    err_[i] = e1*k1_[i] + e2*k2_[i] + e3*k3_[i];
  }
  return normalizeError(y0, y, err_);
}


void mousse::Rosenbrock23::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}

