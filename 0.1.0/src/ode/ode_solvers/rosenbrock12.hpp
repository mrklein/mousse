// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Rosenbrock12
// Description
//   L-stable embedded Rosenbrock ODE solver of order (1)2.
//   References:
//   \verbatim
//     "A second-order Rosenbrock method applied to
//     photochemical dispersion problems",
//     J. G. Verwer,
//     E. J. Spee,
//     J. G. Blom,
//     W. Hundsdorfer,
//     Siam Journal on Scientific Computing 01/1999; 20(4):1456-1480.
//   \endverbatim
// SourceFiles
//   rosenbrock12.cpp
#ifndef rosenbrock12_hpp_
#define rosenbrock12_hpp_
#include "ode_solver.hpp"
#include "adaptive_solver.hpp"
namespace mousse
{
class Rosenbrock12
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField k1_;
    mutable scalarField k2_;
    mutable scalarField err_;
    mutable scalarField dydx_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable scalarSquareMatrix a_;
    mutable labelList pivotIndices_;
    static const scalar
      a21,
      c21,
      b1, b2,
      gamma,
      c2,
      e1, e2,
      d1, d2;
public:
  //- Runtime type information
  TYPE_NAME("Rosenbrock12");
  // Constructors
    //- Construct from ODE
    Rosenbrock12(const ODESystem& ode, const dictionary& dict);
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
