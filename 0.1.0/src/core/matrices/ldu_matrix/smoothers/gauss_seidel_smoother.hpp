#ifndef CORE_MATRICES_LDU_MATRIX_SMOOTHERS_GAUSS_SEIDEL_SMOOTHER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SMOOTHERS_GAUSS_SEIDEL_SMOOTHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GaussSeidelSmoother
// Description
//   A lduMatrix::smoother for Gauss-Seidel

#include "ldu_matrix.hpp"


namespace mousse {

class GaussSeidelSmoother
:
  public lduMatrix::smoother
{
public:
  //- Runtime type information
  TYPE_NAME("GaussSeidel");
  // Constructors
    //- Construct from components
    GaussSeidelSmoother
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
