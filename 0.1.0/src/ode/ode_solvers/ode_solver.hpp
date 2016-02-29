#ifndef ODE_ODE_SOLVERS_ODE_SOLVER_HPP_
#define ODE_ODE_SOLVERS_ODE_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ODESolver
// Description
//   Abstract base-class for ODE system solvers
// SourceFiles
//   ode_solver.cpp
#include "ode_system.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class ODESolver
{
protected:
  // Protected data
    //- Reference to ODESystem
    const ODESystem& odes_;
    //- Size of the ODESystem
    label n_;
    //- Absolute convergence tolerance per step
    scalarField absTol_;
    //- Relative convergence tolerance per step
    scalarField relTol_;
    //- The maximum number of sub-steps allowed for the integration step
    label maxSteps_;
  // Protected Member Functions
    //- Return the nomalized scalar error
    scalar normalizeError
    (
      const scalarField& y0,
      const scalarField& y,
      const scalarField& err
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("ODESolver");
  class stepState
  {
    public:
    const bool forward;
    scalar dxTry;
    scalar dxDid;
    bool first;
    bool last;
    bool reject;
    bool prevReject;
    stepState(const scalar dx)
    :
      forward{dx > 0 ? true : false},
      dxTry{dx},
      dxDid{0},
      first{true},
      last{false},
      reject{false},
      prevReject{false}
    {}
  };
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      ODESolver,
      dictionary,
      (const ODESystem& ode, const dictionary& dict),
      (ode, dict)
    );
  // Constructors
    //- Construct for given ODESystem
    ODESolver(const ODESystem& ode, const dictionary& dict);
    //- Construct for given ODESystem specifying tolerances
    ODESolver
    (
      const ODESystem& ode,
      const scalarField& absTol,
      const scalarField& relTol
    );
    //- Disallow default bitwise copy construct
    ODESolver(const ODESolver&) = delete;
    //- Disallow default bitwise assignment
    ODESolver& operator=(const ODESolver&) = delete;
  // Selectors
    //- Select null constructed
    static autoPtr<ODESolver> New
    (
      const ODESystem& ode,
      const dictionary& dict
    );
  //- Destructor
  virtual ~ODESolver()
  {}
  // Member Functions
    scalarField& absTol()
    {
      return absTol_;
    }
    scalarField& relTol()
    {
      return relTol_;
    }
    //- Solve the ODE system as far as possible upto dxTry
    //  adjusting the step as necessary to provide a solution within
    //  the specified tolerance.
    //  Update the state and return an estimate for the next step in dxTry
    virtual void solve
    (
      scalar& x,
      scalarField& y,
      scalar& dxTry
    ) const //= 0;
    {
      stepState step(dxTry);
      solve(x, y, step);
      dxTry = step.dxTry;
    }
    //- Solve the ODE system as far as possible upto dxTry
    //  adjusting the step as necessary to provide a solution within
    //  the specified tolerance.
    //  Update the state and return an estimate for the next step in dxTry
    virtual void solve
    (
      scalar& x,
      scalarField& y,
      stepState& step
    ) const;
    //- Solve the ODE system from xStart to xEnd, update the state
    //  and return an estimate for the next step in dxTry
    virtual void solve
    (
      const scalar xStart,
      const scalar xEnd,
      scalarField& y,
      scalar& dxEst
    ) const;
};
}  // namespace mousse
#endif
