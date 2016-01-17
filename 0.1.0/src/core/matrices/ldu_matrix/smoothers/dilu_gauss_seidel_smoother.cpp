// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dilu_gauss_seidel_smoother.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(DILUGaussSeidelSmoother, 0);
  lduMatrix::smoother::
    addasymMatrixConstructorToTable<DILUGaussSeidelSmoother>
    addDILUGaussSeidelSmootherAsymMatrixConstructorToTable_;
}
// Constructors 
mousse::DILUGaussSeidelSmoother::DILUGaussSeidelSmoother
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces
)
:
  lduMatrix::smoother
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  ),
  diluSmoother_
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  ),
  gsSmoother_
  (
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  )
{}
// Member Functions 
void mousse::DILUGaussSeidelSmoother::smooth
(
  scalarField& psi,
  const scalarField& source,
  const direction cmpt,
  const label nSweeps
) const
{
  diluSmoother_.smooth(psi, source, cmpt, nSweeps);
  gsSmoother_.smooth(psi, source, cmpt, nSweeps);
}
