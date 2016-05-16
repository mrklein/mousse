// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rkdp45.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(RKDP45, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, RKDP45, dictionary);

const scalar RKDP45::c2 = 1.0/5.0;
const scalar RKDP45::c3 = 3.0/10.0;
const scalar RKDP45::c4 = 4.0/5.0;
const scalar RKDP45::c5 = 8.0/9.0;
const scalar RKDP45::a21 = 1.0/5.0;
const scalar RKDP45::a31 = 3.0/40.0;
const scalar RKDP45::a32 = 9.0/40.0;
const scalar RKDP45::a41 = 44.0/45.0;
const scalar RKDP45::a42 = -56.0/15.0;
const scalar RKDP45::a43 = 32.0/9.0;
const scalar RKDP45::a51 = 19372.0/6561.0;
const scalar RKDP45::a52 = -25360.0/2187.0;
const scalar RKDP45::a53 = 64448.0/6561.0;
const scalar RKDP45::a54 = -212.0/729.0;
const scalar RKDP45::a61 = 9017.0/3168.0;
const scalar RKDP45::a62 = -355.0/33.0;
const scalar RKDP45::a63 = 46732.0/5247.0;
const scalar RKDP45::a64 = 49.0/176.0;
const scalar RKDP45::a65 = -5103.0/18656.0;
const scalar RKDP45::b1 = 35.0/384.0;
const scalar RKDP45::b3 = 500.0/1113.0;
const scalar RKDP45::b4 = 125.0/192.0;
const scalar RKDP45::b5 = -2187.0/6784.0;
const scalar RKDP45::b6 = 11.0/84.0;
const scalar RKDP45::e1 = 5179.0/57600.0 - RKDP45::b1;
const scalar RKDP45::e3 = 7571.0/16695.0 - RKDP45::b3;
const scalar RKDP45::e4 = 393.0/640.0 - RKDP45::b4;
const scalar RKDP45::e5 = -92097.0/339200.0 - RKDP45::b5;
const scalar RKDP45::e6 = 187.0/2100.0 - RKDP45::b6;
const scalar RKDP45::e7 = 1.0/40.0;

}


// Constructors 
mousse::RKDP45::RKDP45(const ODESystem& ode, const dictionary& dict)
:
  ODESolver{ode, dict},
  adaptiveSolver{ode, dict},
  yTemp_{n_},
  k2_{n_},
  k3_{n_},
  k4_{n_},
  k5_{n_},
  k6_{n_},
  err_{n_}
{}


// Member Functions 
mousse::scalar mousse::RKDP45::solve
(
  const scalar x0,
  const scalarField& y0,
  const scalarField& dydx0,
  const scalar dx,
  scalarField& y
) const
{
  FOR_ALL(yTemp_, i) {
    yTemp_[i] = y0[i] + a21*dx*dydx0[i];
  }
  odes_.derivatives(x0 + c2*dx, yTemp_, k2_);
  FOR_ALL(yTemp_, i) {
    yTemp_[i] = y0[i] + dx*(a31*dydx0[i] + a32*k2_[i]);
  }
  odes_.derivatives(x0 + c3*dx, yTemp_, k3_);
  FOR_ALL(yTemp_, i) {
    yTemp_[i] = y0[i] + dx*(a41*dydx0[i] + a42*k2_[i] + a43*k3_[i]);
  }
  odes_.derivatives(x0 + c4*dx, yTemp_, k4_);
  FOR_ALL(yTemp_, i) {
    yTemp_[i] = y0[i]
      + dx*(a51*dydx0[i] + a52*k2_[i] + a53*k3_[i] + a54*k4_[i]);
  }
  odes_.derivatives(x0 + c5*dx, yTemp_, k5_);
  FOR_ALL(yTemp_, i) {
    yTemp_[i] = y0[i]
      + dx*(a61*dydx0[i] + a62*k2_[i] + a63*k3_[i] + a64*k4_[i] + a65*k5_[i]);
  }
  odes_.derivatives(x0 + dx, yTemp_, k6_);
  FOR_ALL(y, i) {
    y[i] = y0[i]
      + dx*(b1*dydx0[i] + b3*k3_[i] + b4*k4_[i] + b5*k5_[i] + b6*k6_[i]);
  }
  // Reuse k2_ for the derivative of the new state
  odes_.derivatives(x0 + dx, y, k2_);
  FOR_ALL(err_, i) {
    err_[i] =
      dx*(e1*dydx0[i] + e3*k3_[i] + e4*k4_[i] + e5*k5_[i] + e6*k6_[i]
          + e7*k2_[i]);
  }
  return normalizeError(y0, y, err_);
}


void mousse::RKDP45::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}
