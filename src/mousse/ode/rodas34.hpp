#ifndef ODE_ODE_SOLVERS_RODAS34_HPP_
#define ODE_ODE_SOLVERS_RODAS34_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rodas34
// Description
//   L-stable, stiffly-accurate embedded Rosenbrock ODE solver of order (3)4.
//   References:
//   \verbatim
//     "Solving Ordinary Differential Equations II: Stiff
//     and Differential-Algebraic Problems, second edition",
//     Hairer, E.,
//     Nørsett, S.,
//     Wanner, G.,
//     Springer-Verlag, Berlin. 1996.
//   \endverbatim

#include "ode_solver.hpp"
#include "adaptive_solver.hpp"


namespace mousse {

class rodas34
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField k1_;
    mutable scalarField k2_;
    mutable scalarField k3_;
    mutable scalarField k4_;
    mutable scalarField k5_;
    mutable scalarField dy_;
    mutable scalarField err_;
    mutable scalarField dydx_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable scalarSquareMatrix a_;
    mutable labelList pivotIndices_;
    static const scalar
      c2, c3, c4,
      d1, d2, d3, d4,
      a21, a31, a32,
      a41, a42, a43,
      a51, a52, a53, a54,
      c21, c31, c32,
      c41, c42, c43,
      c51, c52, c53, c54,
      c61, c62, c63, c64, c65,
      gamma;
public:
  //- Runtime type information
  TYPE_NAME("rodas34");
  // Constructors
    //- Construct from ODE
    rodas34(const ODESystem& ode, const dictionary& dict);
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

