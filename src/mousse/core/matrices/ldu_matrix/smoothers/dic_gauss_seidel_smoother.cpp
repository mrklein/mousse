// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dic_gauss_seidel_smoother.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(DICGaussSeidelSmoother, 0);
lduMatrix::smoother::addsymMatrixConstructorToTable<DICGaussSeidelSmoother>
  addDICGaussSeidelSmootherSymMatrixConstructorToTable_;

}


// Constructors 
mousse::DICGaussSeidelSmoother::DICGaussSeidelSmoother
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces
)
:
  lduMatrix::smoother
  {
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  },
  dicSmoother_
  {
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  },
  gsSmoother_
  {
    fieldName,
    matrix,
    interfaceBouCoeffs,
    interfaceIntCoeffs,
    interfaces
  }
{}


// Member Functions 
void mousse::DICGaussSeidelSmoother::smooth
(
  scalarField& psi,
  const scalarField& source,
  const direction cmpt,
  const label nSweeps
) const
{
  dicSmoother_.smooth(psi, source, cmpt, nSweeps);
  gsSmoother_.smooth(psi, source, cmpt, nSweeps);
}
