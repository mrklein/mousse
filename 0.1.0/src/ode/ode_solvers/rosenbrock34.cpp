// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rosenbrock34.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(Rosenbrock34, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(ODESolver, Rosenbrock34, dictionary);
const scalar
  // Constants by Shampine
  // More accurate than the L-Stable coefficients for small step-size
  // but less stable for large step-size
  Rosenbrock34::a21 = 2,
  Rosenbrock34::a31 = 48.0/25.0,
  Rosenbrock34::a32 = 6.0/25.0,
  Rosenbrock34::c21 = -8,
  Rosenbrock34::c31 = 372.0/25.0,
  Rosenbrock34::c32 = 12.0/5.0,
  Rosenbrock34::c41 = -112.0/125.0,
  Rosenbrock34::c42 = -54.0/125.0,
  Rosenbrock34::c43 = -2.0/5.0,
  Rosenbrock34::b1 = 19.0/9.0,
  Rosenbrock34::b2 = 1.0/2.0,
  Rosenbrock34::b3 = 25.0/108.0,
  Rosenbrock34::b4 = 125.0/108.0,
  Rosenbrock34::e1 = 34.0/108.0,
  Rosenbrock34::e2 = 7.0/36.0,
  Rosenbrock34::e3 = 0,
  Rosenbrock34::e4 = 125.0/108.0,
  Rosenbrock34::gamma = 1.0/2.0,
  Rosenbrock34::c2 = 1,
  Rosenbrock34::c3  = 3.0/5.0,
  Rosenbrock34::d1 = 1.0/2.0,
  Rosenbrock34::d2 = -3.0/2.0,
  Rosenbrock34::d3 = 605.0/250.0,
  Rosenbrock34::d4 = 29.0/250.0;
  /*
  // L-Stable constants from Hairer et. al.
  Rosenbrock34::a21 = 2,
  Rosenbrock34::a31 = 1.867943637803922,
  Rosenbrock34::a32 = 0.2344449711399156,
  Rosenbrock34::c21 = -7.137615036412310,
  Rosenbrock34::c31 = 2.580708087951457,
  Rosenbrock34::c32 = 0.6515950076447975,
  Rosenbrock34::c41 = -2.137148994382534,
  Rosenbrock34::c42 = -0.3214669691237626,
  Rosenbrock34::c43 = -0.6949742501781779,
  Rosenbrock34::b1 = 2.255570073418735,
  Rosenbrock34::b2 = 0.2870493262186792,
  Rosenbrock34::b3 = 0.435317943184018,
  Rosenbrock34::b4 = 1.093502252409163,
  Rosenbrock34::e1 = -0.2815431932141155,
  Rosenbrock34::e2 = -0.0727619912493892,
  Rosenbrock34::e3 = -0.1082196201495311,
  Rosenbrock34::e4 = -1.093502252409163,
  Rosenbrock34::gamma = 0.57282,
  Rosenbrock34::c2 = 1.14564,
  Rosenbrock34::c3 = 0.65521686381559,
  Rosenbrock34::d1 = 0.57282,
  Rosenbrock34::d2 = -1.769193891319233,
  Rosenbrock34::d3 = 0.7592633437920482,
  Rosenbrock34::d4 = -0.1049021087100450;
  */
}
// Constructors 
mousse::Rosenbrock34::Rosenbrock34(const ODESystem& ode, const dictionary& dict)
:
  ODESolver(ode, dict),
  adaptiveSolver(ode, dict),
  k1_(n_),
  k2_(n_),
  k3_(n_),
  k4_(n_),
  err_(n_),
  dydx_(n_),
  dfdx_(n_),
  dfdy_(n_, n_),
  a_(n_, n_),
  pivotIndices_(n_)
{}
// Member Functions 
mousse::scalar mousse::Rosenbrock34::solve
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
    a_[i][i] += 1.0/(gamma*dx);
  }
  LUDecompose(a_, pivotIndices_);
  // Calculate k1:
  FOR_ALL(k1_, i)
  {
    k1_[i] = dydx0[i] + dx*d1*dfdx_[i];
  }
  LUBacksubstitute(a_, pivotIndices_, k1_);
  // Calculate k2:
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + a21*k1_[i];
  }
  odes_.derivatives(x0 + c2*dx, y, dydx_);
  FOR_ALL(k2_, i)
  {
    k2_[i] = dydx_[i] + dx*d2*dfdx_[i] + c21*k1_[i]/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k2_);
  // Calculate k3:
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + a31*k1_[i] + a32*k2_[i];
  }
  odes_.derivatives(x0 + c3*dx, y, dydx_);
  FOR_ALL(k3_, i)
  {
    k3_[i] = dydx_[i] + dx*d3*dfdx_[i] + (c31*k1_[i] + c32*k2_[i])/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k3_);
  // Calculate k4:
  FOR_ALL(k4_, i)
  {
    k4_[i] = dydx_[i] + dx*d4*dfdx_[i]
     + (c41*k1_[i] + c42*k2_[i] + c43*k3_[i])/dx;
  }
  LUBacksubstitute(a_, pivotIndices_, k4_);
  // Calculate error and update state:
  FOR_ALL(y, i)
  {
    y[i] = y0[i] + b1*k1_[i] + b2*k2_[i] + b3*k3_[i] + b4*k4_[i];
    err_[i] = e1*k1_[i] + e2*k2_[i] + e4*k4_[i];
  }
  return normalizeError(y0, y, err_);
}
void mousse::Rosenbrock34::solve
(
  scalar& x,
  scalarField& y,
  scalar& dxTry
) const
{
  adaptiveSolver::solve(odes_, x, y, dxTry);
}
