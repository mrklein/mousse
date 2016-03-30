#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_FDIC_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_FDIC_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FDICPreconditioner
// Description
//   Faster version of the DICPreconditioner diagonal-based incomplete
//   Cholesky preconditioner for symmetric matrices
//   (symmetric equivalent of DILU) in which the the reciprocal of the
//   preconditioned diagonal and the upper coefficients divided by the diagonal
//   are calculated and stored.

#include "ldu_matrix.hpp"


namespace mousse {

class FDICPreconditioner
:
  public lduMatrix::preconditioner
{
  // Private data

    //- The reciprocal preconditioned diagonal
    scalarField rD_;
    scalarField rDuUpper_;
    scalarField rDlUpper_;

public:

  //- Runtime type information
  TYPE_NAME("FDIC");

  // Constructors

    //- Construct from matrix components and preconditioner solver controls
    FDICPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControlsUnused
    );

    //- Disallow default bitwise copy construct
    FDICPreconditioner(const FDICPreconditioner&) = delete;

    //- Disallow default bitwise assignment
    FDICPreconditioner& operator=(const FDICPreconditioner&) = delete;

  //- Destructor
  virtual ~FDICPreconditioner()
  {}

  // Member Functions

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
