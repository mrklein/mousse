#ifndef ODE_ODE_SOLVERS_ADAPTIVE_SOLVER_HPP_
#define ODE_ODE_SOLVERS_ADAPTIVE_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adaptiveSolver
// Description

#include "ode_solver.hpp"


namespace mousse {

class adaptiveSolver
{
  // Private data
    //- Step-size adjustment controls
    scalar safeScale_, alphaInc_, alphaDec_, minScale_, maxScale_;
    //- Cache for dydx at the initial time
    mutable scalarField dydx0_;
    //- Temprorary for the test-step solution
    mutable scalarField yTemp_;
public:
  // Constructors
    //- Construct from ODESystem
    adaptiveSolver(const ODESystem& ode, const dictionary& dict);
  //- Destructor
  virtual ~adaptiveSolver()
  {}
  // Member Functions
    //- Solve a single step dx and return the error
    virtual scalar solve
    (
      const scalar x0,
      const scalarField& y0,
      const scalarField& dydx0,
      const scalar dx,
      scalarField& y
    ) const = 0;
    //- Solve the ODE system and the update the state
    void solve
    (
      const ODESystem& ode,
      scalar& x,
      scalarField& y,
      scalar& dxTry
    ) const;
};

}  // namespace mousse

#endif

