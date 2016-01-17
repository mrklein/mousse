// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_diagonal_preconditioner.hpp"
// Constructors 
template<class Type, class DType, class LUType>
mousse::DiagonalPreconditioner<Type, DType, LUType>::DiagonalPreconditioner
(
  const typename LduMatrix<Type, DType, LUType>::solver& sol,
  const dictionary&
)
:
  LduMatrix<Type, DType, LUType>::preconditioner(sol),
  rD(sol.matrix().diag().size())
{
  DType* __restrict__ rDPtr = rD.begin();
  const DType* __restrict__ DPtr = this->solver_.matrix().diag().begin();
  label nCells = rD.size();
  // Generate inverse (reciprocal for scalar) diagonal
  for (label cell=0; cell<nCells; cell++)
  {
    rDPtr[cell] = inv(DPtr[cell]);
  }
}
// Member Functions 
template<class Type, class DType, class LUType>
void mousse::DiagonalPreconditioner<Type, DType, LUType>::read(const dictionary&)
{}
template<class Type, class DType, class LUType>
void mousse::DiagonalPreconditioner<Type, DType, LUType>::precondition
(
  Field<Type>& wA,
  const Field<Type>& rA
) const
{
  Type* __restrict__ wAPtr = wA.begin();
  const Type* __restrict__ rAPtr = rA.begin();
  const DType* __restrict__ rDPtr = rD.begin();
  label nCells = wA.size();
  for (label cell=0; cell<nCells; cell++)
  {
    wAPtr[cell] = dot(rDPtr[cell], rAPtr[cell]);
  }
}
