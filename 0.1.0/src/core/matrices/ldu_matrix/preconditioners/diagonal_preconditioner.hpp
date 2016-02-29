#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DIAGONAL_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DIAGONAL_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diagonalPreconditioner
// Description
//   Diagonal preconditioner for both symmetric and asymmetric matrices.
//   The reciprocal of the diagonal is calculated and stored for reuse
//   because on most systems '*' is faster than '/'.
// SourceFiles
//   diagonal_preconditioner.cpp


#include "ldu_matrix.hpp"

namespace mousse
{

class diagonalPreconditioner
:
  public lduMatrix::preconditioner
{
  // Private data

    //- The reciprocal diagonal
    scalarField rD;

public:

  //- Runtime type information
  TYPE_NAME("diagonal");

  // Constructors

    //- Construct from matrix components and preconditioner solver controls
    diagonalPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControlsUnused
    );

    //- Disallow default bitwise copy construct
    diagonalPreconditioner(const diagonalPreconditioner&) = delete;

    //- Disallow default bitwise assignment
    diagonalPreconditioner& operator=(const diagonalPreconditioner&) = delete;

  //- Destructor
  virtual ~diagonalPreconditioner()
  {}

  // Member Functions

    //- Return wA the preconditioned form of residual rA
    virtual void precondition
    (
      scalarField& wA,
      const scalarField& rA,
      const direction cmpt=0
    ) const;

    //- Return wT the transpose-matrix preconditioned form of residual rT.
    virtual void preconditionT
    (
      scalarField& wT,
      const scalarField& rT,
      const direction cmpt=0
    ) const
    {
      return precondition(wT, rT, cmpt);
    }

};

}  // namespace mousse

#endif
