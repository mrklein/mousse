// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_preconditioner.hpp"
// Constructors 
template<class Type, class DType, class LUType>
mousse::NoPreconditioner<Type, DType, LUType>::NoPreconditioner
(
  const typename LduMatrix<Type, DType, LUType>::solver& sol,
  const dictionary&
)
:
  LduMatrix<Type, DType, LUType>::preconditioner(sol)
{}
// Member Functions 
template<class Type, class DType, class LUType>
void mousse::NoPreconditioner<Type, DType, LUType>::read(const dictionary&)
{}
template<class Type, class DType, class LUType>
void mousse::NoPreconditioner<Type, DType, LUType>::precondition
(
  Field<Type>& wA,
  const Field<Type>& rA
) const
{
  wA = rA;
}
