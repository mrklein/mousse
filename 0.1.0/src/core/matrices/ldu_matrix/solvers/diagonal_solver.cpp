// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diagonal_solver.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(diagonalSolver, 0);
}
// Constructors 
mousse::diagonalSolver::diagonalSolver
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const dictionary& solverControls
)
:
  lduMatrix::solver
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverControls
  )
{}
mousse::solverPerformance mousse::diagonalSolver::solve
(
  scalarField& psi,
  const scalarField& source,
  const direction /*cmpt*/
) const
{
  psi = source/matrix_.diag();
  return solverPerformance
  (
    typeName,
    fieldName_,
    0,
    0,
    0,
    true,
    false
  );
}
