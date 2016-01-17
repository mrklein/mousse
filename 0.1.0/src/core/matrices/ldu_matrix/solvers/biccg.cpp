// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "biccg.hpp"
#include "istring_stream.hpp"

// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(BICCG, 0);
lduMatrix::solver::addasymMatrixConstructorToTable<BICCG>
  addBICCGSymMatrixConstructorToTable_;
}

// Static Member Functions
mousse::dictionary mousse::BICCG::solverDict
(
  const scalar tol,
  const scalar relTol
)
{
  dictionary dict(IStringStream("solver PBiCG; preconditioner DILU;")());
  dict.add("tolerance", tol);
  dict.add("relTol", relTol);
  return dict;
}
mousse::dictionary mousse::BICCG::solverDict
(
  Istream& is
)
{
  scalar tol(readScalar(is));
  scalar relTol(readScalar(is));
  return solverDict(tol, relTol);
}

// Constructors
mousse::BICCG::BICCG
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const dictionary& solverControls
)
:
  PBiCG
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverControls
  )
{}
mousse::BICCG::BICCG
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const scalar tolerance,
  const scalar relTol
)
:
  PBiCG
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverDict(tolerance, relTol)
  )
{}
