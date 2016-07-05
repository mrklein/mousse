#ifndef CORE_MATRICES_TLDU_MATRIX_TPRECONDITIONERS_TDIAGONAL_PRECONDITIONER_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TPRECONDITIONERS_TDIAGONAL_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DiagonalPreconditioner
// Description
//   Diagonal preconditioner for both symmetric and asymmetric matrices.
//   The inverse (reciprocal for scalar) of the diagonal is calculated and
//   stored.

#include "_ldu_matrix.hpp"


namespace mousse {

template<class Type, class DType, class LUType>
class DiagonalPreconditioner
:
  public LduMatrix<Type, DType, LUType>::preconditioner
{
  // Private data

    //- The inverse (reciprocal for scalar) diagonal
    Field<DType> rD;

public:

  //- Runtime type information
  TYPE_NAME("diagonal");

  // Constructors

    //- Construct from matrix components and preconditioner data dictionary
    DiagonalPreconditioner
    (
      const typename LduMatrix<Type, DType, LUType>::solver& sol,
      const dictionary& preconditionerDict
    );

    //- Disallow default bitwise copy construct
    DiagonalPreconditioner(const DiagonalPreconditioner&) = delete;

    //- Disallow default bitwise assignment
    DiagonalPreconditioner& operator=(const DiagonalPreconditioner&) = delete;

  // Destructor
    virtual ~DiagonalPreconditioner()
    {}

  // Member Functions

    //- Read and reset the preconditioner parameters from the given
    //  dictionary
    virtual void read(const dictionary& preconditionerDict);

    //- Return wA the preconditioned form of residual rA
    virtual void precondition
    (
      Field<Type>& wA,
      const Field<Type>& rA
    ) const;

    //- Return wT the transpose-matrix preconditioned form of
    //  residual rT.
    virtual void preconditionT
    (
      Field<Type>& wT,
      const Field<Type>& rT
    ) const
    {
      return(precondition(wT, rT));
    }

};

}  // namespace mousse


#include "_diagonal_preconditioner.ipp"

#endif
