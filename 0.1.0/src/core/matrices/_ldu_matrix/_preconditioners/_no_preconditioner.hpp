// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoPreconditioner
// Description
//   Null preconditioner for both symmetric and asymmetric matrices.
// SourceFiles
//   no_preconditioner.cpp

#ifndef _no_preconditioner_hpp_
#define _no_preconditioner_hpp_

#include "_ldu_matrix.hpp"

namespace mousse
{
template<class Type, class DType, class LUType>
class NoPreconditioner
:
  public LduMatrix<Type, DType, LUType>::preconditioner
{
public:

  //- Runtime type information
  TYPE_NAME("none");

  // Constructors

    //- Construct from matrix components and preconditioner data dictionary
    NoPreconditioner
    (
      const typename LduMatrix<Type, DType, LUType>::solver& sol,
      const dictionary& preconditionerDict
    );

    //- Disallow default bitwise copy construct
    NoPreconditioner(const NoPreconditioner&) = delete;

    //- Disallow default bitwise assignment
    NoPreconditioner& operator=(const NoPreconditioner&) = delete;

  // Destructor
    virtual ~NoPreconditioner()
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

#ifdef NoRepository
#   include "_no_preconditioner.cpp"
#endif
#endif
