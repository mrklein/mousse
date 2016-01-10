// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_diagonal_solver.hpp"
// Constructors 
template<class Type, class DType, class LUType>
mousse::DiagonalSolver<Type, DType, LUType>::DiagonalSolver
(
  const word& fieldName,
  const LduMatrix<Type, DType, LUType>& matrix,
  const dictionary& solverDict
)
:
  LduMatrix<Type, DType, LUType>::solver
  (
    fieldName,
    matrix,
    solverDict
  )
{}
template<class Type, class DType, class LUType>
void mousse::DiagonalSolver<Type, DType, LUType>::read
(
  const dictionary&
)
{}
template<class Type, class DType, class LUType>
mousse::SolverPerformance<Type>
mousse::DiagonalSolver<Type, DType, LUType>::solve
(
  Field<Type>& psi
) const
{
  psi = this->matrix_.source()/this->matrix_.diag();
  return SolverPerformance<Type>
  (
    typeName,
    this->fieldName_,
    pTraits<Type>::zero,
    pTraits<Type>::zero,
    0,
    true,
    false
  );
}
