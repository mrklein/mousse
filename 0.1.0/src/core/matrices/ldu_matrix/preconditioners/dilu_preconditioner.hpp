#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DILU_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DILU_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DILUPreconditioner
// Description
//   Simplified diagonal-based incomplete LU preconditioner for asymmetric
//   matrices.  The reciprocal of the preconditioned diagonal is calculated
//   and stored.
// SourceFiles
//   dilu_preconditioner.cpp
#include "ldu_matrix.hpp"
namespace mousse
{
class DILUPreconditioner
:
  public lduMatrix::preconditioner
{
  // Private data
    //- The reciprocal preconditioned diagonal
    scalarField rD_;
public:
  //- Runtime type information
  TYPE_NAME("DILU");
  // Constructors
    //- Construct from matrix components and preconditioner solver controls
    DILUPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControlsUnused
    );
  //- Destructor
  virtual ~DILUPreconditioner()
  {}
  // Member Functions
    //- Calculate the reciprocal of the preconditioned diagonal
    static void calcReciprocalD(scalarField& rD, const lduMatrix& matrix);
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
    ) const;
};
}  // namespace mousse
#endif
