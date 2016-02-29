#ifndef CORE_MATRICES_LDU_MATRIX_SMOOTHERS_NON_BLOCKING_GAUSS_SEIDEL_SMOOTHER_HPP_
#define CORE_MATRICES_LDU_MATRIX_SMOOTHERS_NON_BLOCKING_GAUSS_SEIDEL_SMOOTHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonBlockingGaussSeidelSmoother
// Description
//   Variant of gaussSeidelSmoother that expects processor boundary
//   cells to be sorted last and so can block later. Only when the
//   cells are actually visited does it need the results to be present.
//   It is expected that there is little benefit to be gained from doing
//   this on a patch by patch basis since the number of processor interfaces
//   is quite small and the overhead of checking whether a processor interface
//   is finished might be quite high (call into mpi). Also this would
//   require a dynamic memory allocation to store the state of the outstanding
//   requests.
// SourceFiles
//   non_blocking_gauss_seidel_smoother.cpp
#include "ldu_matrix.hpp"
namespace mousse
{
class nonBlockingGaussSeidelSmoother
:
  public lduMatrix::smoother
{
  // Private data
    //- Starting cell when to block
    label blockStart_;
public:
  //- Runtime type information
  TYPE_NAME("nonBlockingGaussSeidel");
  // Constructors
    //- Construct from components
    nonBlockingGaussSeidelSmoother
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
      const label blockStart,
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
