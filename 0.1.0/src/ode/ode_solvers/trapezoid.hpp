// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Trapezoid
// Description
//   Trapezoidal ODE solver of order (1)2.
// SourceFiles
//   trapezoid.cpp
#ifndef trapezoid_hpp_
#define trapezoid_hpp_
#include "ode_solver.hpp"
#include "adaptive_solver.hpp"
namespace mousse
{
class Trapezoid
:
  public ODESolver,
  public adaptiveSolver
{
  // Private data
    mutable scalarField err_;
public:
  //- Runtime type information
  TypeName("Trapezoid");
  // Constructors
    //- Construct from ODE
    Trapezoid(const ODESystem& ode, const dictionary& dict);
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
