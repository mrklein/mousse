// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::smoothSolver
// Description
//   Iterative solver for symmetric and asymetric matrices which uses a
//   run-time selected smoother e.g. GaussSeidel to converge the solution to
//   the required tolerance.
//   To improve efficiency, the residual is evaluated after every nSweeps
//   smoothing iterations.
// SourceFiles
//   smooth_solver.cpp
#ifndef smooth_solver_hpp_
#define smooth_solver_hpp_
#include "ldu_matrix.hpp"
namespace mousse
{
class smoothSolver
:
  public lduMatrix::solver
{
protected:
  // Protected data
    //- Number of sweeps before the evaluation of residual
    label nSweeps_;
    //- Read the control parameters from the controlDict_
    virtual void readControls();
public:
  //- Runtime type information
  TYPE_NAME("smoothSolver");
  // Constructors
    //- Construct from matrix components and solver controls
    smoothSolver
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );
  //- Destructor
  virtual ~smoothSolver()
  {}
  // Member Functions
    //- Solve the matrix with this solver
    virtual solverPerformance solve
    (
      scalarField& psi,
      const scalarField& source,
      const direction cmpt=0
    ) const;
};
}  // namespace mousse
#endif
