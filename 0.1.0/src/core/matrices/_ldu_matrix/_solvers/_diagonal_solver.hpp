// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DiagonalSolver
// Description
//   mousse::DiagonalSolver
// SourceFiles
//   diagonal_solver.cpp

#ifndef _diagonal_solver_hpp_
#define _diagonal_solver_hpp_

#include "_ldu_matrix.hpp"

namespace mousse
{
template<class Type, class DType, class LUType>
class DiagonalSolver
:
  public LduMatrix<Type, DType, LUType>::solver
{
public:

  //- Runtime type information
  TYPE_NAME("diagonal");

  // Constructors

    //- Construct from matrix
    DiagonalSolver
    (
      const word& fieldName,
      const LduMatrix<Type, DType, LUType>& matrix,
      const dictionary& solverDict
    );

    //- Disallow default bitwise copy construct
    DiagonalSolver(const DiagonalSolver&) = delete;

    //- Disallow default bitwise assignment
    DiagonalSolver& operator=(const DiagonalSolver&) = delete;

  // Member Functions

    //- Read and reset the solver parameters from the given dictionary
    void read(const dictionary& solverDict);

    //- Solve the matrix with this solver
    virtual SolverPerformance<Type> solve(Field<Type>& psi) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "_diagonal_solver.cpp"
#endif
#endif
