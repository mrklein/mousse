// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iccg.hpp"
#include "istring_stream.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(ICCG, 0);
lduMatrix::solver::addsymMatrixConstructorToTable<ICCG>
  addICCGSymMatrixConstructorToTable_;

}


// Static Member Functions
mousse::dictionary mousse::ICCG::solverDict
(
  const scalar tol,
  const scalar relTol
)
{
  dictionary dict(IStringStream("solver PCG; preconditioner DIC;")());
  dict.add("tolerance", tol);
  dict.add("relTol", relTol);
  return dict;
}


mousse::dictionary mousse::ICCG::solverDict
(
  Istream& is
)
{
  scalar tol{readScalar(is)};
  scalar relTol{readScalar(is)};
  return solverDict(tol, relTol);
}


// Constructors
mousse::ICCG::ICCG
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const dictionary& solverControls
)
:
  PCG
  {
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverControls
  }
{}


mousse::ICCG::ICCG
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
  PCG
  {
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces,
    solverDict(tolerance, relTol)
  }
{}
