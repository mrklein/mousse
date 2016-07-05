#ifndef ODE_ODE_SOLVERS_RODAS23_HPP_
#define ODE_ODE_SOLVERS_RODAS23_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rodas23
// Description
//   L-stable, stiffly-accurate embedded Rosenbrock ODE solver of order (2)3.
//   References:
//   \verbatim
//     Sandu et al,
//     "Benchmarking stiff ODE solvers for atmospheric chemistry problems II
//     Rosenbrock solvers",
//     A. Sandu,
//     J.G. Verwer,
//     J.G. Blom,
//     E.J. Spee,
//     G.R. Carmichael,
//     F.A. Potra,
//     Atmospheric Environment, Volume 31, 1997, Issue 20, Pages 3459-3472
//   \endverbatim

#include "ode_solver.hpp"
#include "adaptive_solver.hpp"


namespace mousse {

class rodas23
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField k1_;
    mutable scalarField k2_;
    mutable scalarField k3_;
    mutable scalarField dy_;
    mutable scalarField err_;
    mutable scalarField dydx_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable scalarSquareMatrix a_;
    mutable labelList pivotIndices_;
    static const scalar
      c3,
      d1, d2,
      a31,
      a41,
      c21, c31, c32,
      c41, c42, c43,
      gamma;

public:
  //- Runtime type information
  TYPE_NAME("rodas23");
  // Constructors
    //- Construct from ODE
    rodas23(const ODESystem& ode, const dictionary& dict);
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

