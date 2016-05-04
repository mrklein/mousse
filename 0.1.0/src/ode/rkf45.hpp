#ifndef ODE_ODE_SOLVERS_RKF45_HPP_
#define ODE_ODE_SOLVERS_RKF45_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RKF45
// Description
//   4/5th Order Runge-Kutta-Fehlberg ODE solver
//   References:
//   \verbatim
//     "Low-order classical Runge-Kutta formulas with step size control
//     and their application to some heat transfer problems."
//     Fehlberg, E.,
//     NASA Technical Report 315, 1969.
//     "Solving Ordinary Differential Equations I: Nonstiff Problems,
//     second edition",
//     Hairer, E.,
//     Nørsett, S.,
//     Wanner, G.,
//     Springer-Verlag, Berlin. 1993, ISBN 3-540-56670-8.
//   \endverbatim
//   This method embedds the 4-th order integration step into the 5-th order step
//   and allows to perform an adapdive step-size control using these two order
//   without the need of re-evaluation.

#include "ode_solver.hpp"
#include "adaptive_solver.hpp"


namespace mousse {

class RKF45
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    //- RKF45 Constants
    static const scalar
      c2, c3, c4, c5, c6,
      a21, a31, a32, a41, a42, a43, a51, a52, a53, a54,
      a61, a62, a63, a64, a65,
      b1, b3, b4, b5, b6,
      e1, e3, e4, e5, e6;
    // Temporary fields
    mutable scalarField yTemp_;
    mutable scalarField k2_;
    mutable scalarField k3_;
    mutable scalarField k4_;
    mutable scalarField k5_;
    mutable scalarField k6_;
    //- Error-estimate field
    mutable scalarField err_;
public:
  //- Runtime type information
  TYPE_NAME("RKF45");
  // Constructors
    //- Construct from ODE
    RKF45(const ODESystem& ode, const dictionary& dict);
  // Member Functions
    //- Inherit solve from ODESolver
    using ODESolver::solve;
    //- Solve a single step dx and return the error
    scalar solve
    (
      const scalar x0,
      const scalarField& y0,
      const scalarField& dydx0,
      const scalar dx,
      scalarField& y
    ) const;
    //- Solve the ODE system and the update the state
    void solve
    (
      scalar& x,
      scalarField& y,
      scalar& dxTry
    ) const;
};

}  // namespace mousse

#endif

