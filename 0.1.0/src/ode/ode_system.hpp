#ifndef ODE_ODE_SYSTEM_HPP_
#define ODE_ODE_SYSTEM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ODESystem
// Description
//   Abstract base class for the systems of ordinary differential equations.

#include "scalar_field.hpp"
#include "scalar_matrices.hpp"


namespace mousse {

class ODESystem
{
public:
  // Constructors
    //- Construct null
    ODESystem()
    {}
  //- Destructor
  virtual ~ODESystem()
  {}
  // Member Functions
    //- Return the number of equations in the system
    virtual label nEqns() const = 0;
    //- Calculate the derivatives in dydx
    virtual void derivatives
    (
      const scalar x,
      const scalarField& y,
      scalarField& dydx
    ) const = 0;
    //- Calculate the Jacobian of the system
    //  Need by the stiff-system solvers
    virtual void jacobian
    (
      const scalar x,
      const scalarField& y,
      scalarField& dfdx,
      scalarSquareMatrix& dfdy
    ) const = 0;
};

}  // namespace mousse

#endif

