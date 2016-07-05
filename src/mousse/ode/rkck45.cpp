// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rkck45.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

  DEFINE_TYPE_NAME_AND_DEBUG(RKCK45, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, RKCK45, dictionary);

const scalar
  RKCK45::c2 = 1.0/5.0,
  RKCK45::c3 = 3.0/10.0,
  RKCK45::c4 = 3.0/5.0,
  RKCK45::c5 = 1.0,
  RKCK45::c6 = 7.0/8.0,
  RKCK45::a21 = 1.0/5.0,
  RKCK45::a31 = 3.0/40.0,
  RKCK45::a32 = 9.0/40.0,
  RKCK45::a41 = 3.0/10.0,
  RKCK45::a42 = -9.0/10.0,
  RKCK45::a43 = 6.0/5.0,
  RKCK45::a51 = -11.0/54.0,
  RKCK45::a52 = 5.0/2.0,
  RKCK45::a53 = -70.0/27.0,
  RKCK45::a54 = 35.0/27.0,
  RKCK45::a61 = 1631.0/55296.0,
  RKCK45::a62 = 175.0/512.0,
  RKCK45::a63 = 575.0/13824.0,
  RKCK45::a64 = 44275.0/110592.0,
  RKCK45::a65 = 253.0/4096.0,
  RKCK45::b1 = 37.0/378.0,
  RKCK45::b3 = 250.0/621.0,
  RKCK45::b4 = 125.0/594.0,
  RKCK45::b6 = 512.0/1771.0,
  RKCK45::e1 = RKCK45::b1 - 2825.0/27648.0,
  RKCK45::e3 = RKCK45::b3 - 18575.0/48384.0,
  RKCK45::e4 = RKCK45::b4 - 13525.0/55296.0,
  RKCK45::e5 = -277.00/14336.0,
  RKCK45::e6 = RKCK45::b6 - 0.25;
}


// Constructors 
mousse::RKCK45::RKCK45(const ODESystem& ode, const dictionary& dict)
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
mousse::scalar mousse::RKCK45::solve
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
  odes_.derivatives(x0 + c6*dx, yTemp_, k6_);
  FOR_ALL(y, i) {
    y[i] = y0[i] + dx*(b1*dydx0[i] + b3*k3_[i] + b4*k4_[i] + b6*k6_[i]);
  }
  FOR_ALL(err_, i) {
    err_[i] =
      dx *(e1*dydx0[i] + e3*k3_[i] + e4*k4_[i] + e5*k5_[i] + e6*k6_[i]);
  }
  return normalizeError(y0, y, err_);
}


void mousse::RKCK45::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}

