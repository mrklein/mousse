#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_DIAGONAL_SOLVER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_DIAGONAL_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diagonalSolver
// Description
//   mousse::diagonalSolver
// SourceFiles
//   diagonal_solver.cpp


#include "ldu_matrix.hpp"

namespace mousse
{

class diagonalSolver
:
  public lduMatrix::solver
{
public:

  //- Runtime type information
  TYPE_NAME("diagonal");

  // Constructors

    //- Construct from matrix and solver controls
    diagonalSolver
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );

    //- Disallow default bitwise copy construct
    diagonalSolver(const diagonalSolver&) = delete;

    //- Disallow default bitwise assignment
    diagonalSolver& operator=(const diagonalSolver&) = delete;

  // Member Functions

    //- Read and reset the solver parameters from the given stream
    void read(const dictionary&)
    {}

    //- Solve the matrix with this solver
    solverPerformance solve
    (
      scalarField& psi,
      const scalarField& source,
      const direction cmpt=0
    ) const;

};

}  // namespace mousse

#endif
