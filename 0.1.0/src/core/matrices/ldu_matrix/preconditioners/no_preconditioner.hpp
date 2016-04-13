#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_NO_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_NO_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noPreconditioner
// Description
//   Null preconditioner for both symmetric and asymmetric matrices.

#include "ldu_matrix.hpp"


namespace mousse {

class noPreconditioner
:
  public lduMatrix::preconditioner
{
public:

  //- Runtime type information
  TYPE_NAME("none");

  // Constructors
    //- Construct from matrix components and preconditioner solver controls
    noPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControlsUnused
    );

    //- Disallow default bitwise copy construct
    noPreconditioner(const noPreconditioner&) = delete;

    //- Disallow default bitwise assignment
    noPreconditioner& operator=(const noPreconditioner&) = delete;

  //- Destructor
  virtual ~noPreconditioner()
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
