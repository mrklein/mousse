#ifndef ODE_ODE_SOLVERS_ROSENBROCK34_HPP_
#define ODE_ODE_SOLVERS_ROSENBROCK34_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Rosenbrock34
// Description
//   L-stable embedded Rosenbrock ODE solver of order (3)4.
//   \verbatim
//     "Solving Ordinary Differential Equations II: Stiff
//     and Differential-Algebraic Problems, second edition",
//     Hairer, E.,
//     Nørsett, S.,
//     Wanner, G.,
//     Springer-Verlag, Berlin. 1996.
//   \endverbatim
//   The default constants are from:
//   \verbatim
//     "Implementation of Rosenbrock Methods"
//     Shampine, L. F.,
//     ACM Transactions on Mathematical Software, vol. 8, 1982, pp. 93–113.
//   \endverbatim
//   with which the scheme is more accurate than with the L-Stable coefficients
//   for small step-size but less stable for large step-size.
//   The L-Stable scheme constants are provided commented-out in Rosenbrock34.C

#include "ode_solver.hpp"
#include "adaptive_solver.hpp"


namespace mousse {

class Rosenbrock34
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField k1_;
    mutable scalarField k2_;
    mutable scalarField k3_;
    mutable scalarField k4_;
    mutable scalarField err_;
    mutable scalarField dydx_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable scalarSquareMatrix a_;
    mutable labelList pivotIndices_;
    static const scalar
      a21, a31, a32,
      c21, c31, c32,
      c41, c42, c43,
      b1, b2, b3, b4,
      e1, e2, e3, e4,
      gamma,
      c2, c3,
      d1, d2, d3, d4;
public:
  //- Runtime type information
  TYPE_NAME("Rosenbrock34");
  // Constructors
    //- Construct from ODE
    Rosenbrock34(const ODESystem& ode, const dictionary& dict);
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

