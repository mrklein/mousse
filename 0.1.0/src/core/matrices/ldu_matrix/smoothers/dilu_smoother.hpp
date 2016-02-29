#ifndef CORE_MATRICES_LDU_MATRIX_SMOOTHERS_DILU_SMOOTHER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SMOOTHERS_DILU_SMOOTHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DILUSmoother
// Description
//   Simplified diagonal-based incomplete LU smoother for asymmetric matrices.
// SourceFiles
//   dilu_smoother.cpp
#include "ldu_matrix.hpp"
namespace mousse
{
class DILUSmoother
:
  public lduMatrix::smoother
{
  // Private data
    //- The reciprocal preconditioned diagonal
    scalarField rD_;
public:
  //- Runtime type information
  TYPE_NAME("DILU");
  // Constructors
    //- Construct from matrix components
    DILUSmoother
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
