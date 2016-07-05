#ifndef CORE_MATRICES_TLDU_MATRIX_TSOLVERS_TSMOOTH_SOLVER_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TSOLVERS_TSMOOTH_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SmoothSolver
// Description
//   Iterative solver for symmetric and assymetric matrices which uses a
//   run-time selected smoother e.g. GaussSeidel to converge the solution to
//   the required tolerance.  To improve efficiency, the residual is evaluated
//   after every nSweeps smoothing iterations.

#include "ldu_matrix.hpp"


namespace mousse {

template<class Type, class DType, class LUType>
class SmoothSolver
:
  public LduMatrix<Type, DType, LUType>::solver
{
protected:
  // Protected data
    //- Number of sweeps before the evaluation of residual
    label nSweeps_;
    //- Read the control parameters from the controlDict_
    virtual void readControls();
public:
  //- Runtime type information
  TYPE_NAME("SmoothSolver");
  // Constructors
    //- Construct from matrix components and solver data dictionary
    SmoothSolver
    (
      const word& fieldName,
      const LduMatrix<Type, DType, LUType>& matrix,
      const dictionary& solverDict
    );
  // Member Functions
    //- Solve the matrix with this solver
    virtual SolverPerformance<Type> solve(Field<Type>& psi) const;
};
}  // namespace mousse

#include "_smooth_solver.ipp"

#endif
