#ifndef CORE_MATRICES_LDU_MATRIX_SMOOTHERS_FDIC_SMOOTHER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SMOOTHERS_FDIC_SMOOTHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FDICSmoother
// Description
//   Simplified diagonal-based incomplete Cholesky smoother for symmetric
//   matrices.
//   To improve efficiency, the residual is evaluated after every nSweeps
//   sweeps.

#include "ldu_matrix.hpp"


namespace mousse {

class FDICSmoother
:
  public lduMatrix::smoother
{
  // Private data
    //- The reciprocal preconditioned diagonal
    scalarField rD_;
    scalarField rDuUpper_;
    scalarField rDlUpper_;
public:
  //- Runtime type information
  TYPE_NAME("FDIC");
  // Constructors
    //- Construct from matrix components
    FDICSmoother
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
  // Member Functions
    //- Smooth the solution for a given number of sweeps
    void smooth
    (
      scalarField& psi,
      const scalarField& source,
      const direction cmpt,
      const label nSweeps
    ) const;
};
}  // namespace mousse
#endif
