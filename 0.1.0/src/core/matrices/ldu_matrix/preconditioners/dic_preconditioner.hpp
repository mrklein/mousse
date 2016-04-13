#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DIC_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_DIC_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DICPreconditioner
// Description
//   Simplified diagonal-based incomplete Cholesky preconditioner for symmetric
//   matrices (symmetric equivalent of DILU).  The reciprocal of the
//   preconditioned diagonal is calculated and stored.

#include "ldu_matrix.hpp"


namespace mousse {

class DICPreconditioner
:
  public lduMatrix::preconditioner
{
  // Private data
    //- The reciprocal preconditioned diagonal
    scalarField rD_;
public:
  //- Runtime type information
  TYPE_NAME("DIC");
  // Constructors
    //- Construct from matrix components and preconditioner solver controls
    DICPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControlsUnused
    );
  //- Destructor
  virtual ~DICPreconditioner()
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
};
}  // namespace mousse
#endif
