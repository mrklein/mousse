#ifndef CORE_MATRICES_LDU_MATRIX_SMOOTHERS_SYM_GAUSS_SEIDEL_SMOOTHER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SMOOTHERS_SYM_GAUSS_SEIDEL_SMOOTHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symGaussSeidelSmoother
// Description
//   A lduMatrix::smoother for symmetric Gauss-Seidel
// SourceFiles
//   sym_gauss_seidel_smoother.cpp
#include "ldu_matrix.hpp"
namespace mousse
{
class symGaussSeidelSmoother
:
  public lduMatrix::smoother
{
public:
  //- Runtime type information
  TYPE_NAME("symGaussSeidel");
  // Constructors
    //- Construct from components
    symGaussSeidelSmoother
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
  // Member Functions
    //- Smooth for the given number of sweeps
    static void smooth
    (
      const word& fieldName,
      scalarField& psi,
      const lduMatrix& matrix,
      const scalarField& source,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const direction cmpt,
      const label nSweeps
    );
    //- Smooth the solution for a given number of sweeps
    virtual void smooth
    (
      scalarField& psi,
      const scalarField& Source,
      const direction cmpt,
      const label nSweeps
    ) const;
};
}  // namespace mousse
#endif
