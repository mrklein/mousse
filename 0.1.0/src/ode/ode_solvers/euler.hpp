// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Euler
// Description
//   Euler ODE solver of order (0)1.
//   The method calculates the new state from:
//   \f[
//     y_{n+1} = y_n + \delta_x f
//   \f]
//   The error is estimated directly from the change in the solution,
//   i.e. the difference between the 0th and 1st order solutions:
//   \f[
//     err_{n+1} = y_{n+1} - y_n
//   \f]
// SourceFiles
//   euler.cpp
#ifndef euler_hpp_
#define euler_hpp_
#include "ode_solver.hpp"
#include "adaptive_solver.hpp"
namespace mousse
{
class Euler
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField err_;
public:
  //- Runtime type information
  TypeName("Euler");
  // Constructors
    //- Construct from ODE
    Euler(const ODESystem& ode, const dictionary& dict);
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
