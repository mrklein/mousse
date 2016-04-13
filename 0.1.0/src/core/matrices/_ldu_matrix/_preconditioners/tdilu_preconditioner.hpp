#ifndef CORE_MATRICES_TLDU_MATRIX_TPRECONDITIONERS_TDILU_PRECONDITIONER_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TPRECONDITIONERS_TDILU_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TDILUPreconditioner
// Description
//   Simplified diagonal-based incomplete LU preconditioner for asymmetric
//   matrices.
//   The inverse (reciprocal for scalar) of the preconditioned diagonal is
//   calculated and stored.

#include "_ldu_matrix.hpp"


namespace mousse {

template<class Type, class DType, class LUType>
class TDILUPreconditioner
:
  public LduMatrix<Type, DType, LUType>::preconditioner
{
  // Private data
    //- The inverse (reciprocal for scalar) preconditioned diagonal
    Field<DType> rD_;
public:
  //- Runtime type information
  TYPE_NAME("DILU");
  // Constructors
    //- Construct from matrix components and preconditioner data dictionary
    TDILUPreconditioner
    (
      const typename LduMatrix<Type, DType, LUType>::solver& sol,
      const dictionary& preconditionerDict
    );
  // Destructor
    virtual ~TDILUPreconditioner()
    {}
  // Member Functions
    //- Calculate the reciprocal of the preconditioned diagonal
    static void calcInvD
    (
      Field<DType>& rD,
      const LduMatrix<Type, DType, LUType>& matrix
    );
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
    ) const;
};

}  // namespace mousse

#include "tdilu_preconditioner.ipp"

#endif
