// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DILUGaussSeidelSmoother
// Description
//   Combined DILU/GaussSeidel smoother for asymmetric matrices in which
//   DILU smoothing is followed by GaussSeidel to ensure that any "spikes"
//   created by the DILU sweeps are smoothed-out.
// SourceFiles
//   dilu_gauss_seidel_smoother.cpp
#ifndef dilu_gauss_seidel_smoother_hpp_
#define dilu_gauss_seidel_smoother_hpp_
#include "dilu_smoother.hpp"
#include "gauss_seidel_smoother.hpp"
namespace mousse
{
class DILUGaussSeidelSmoother
:
  public lduMatrix::smoother
{
  // Private data
    DILUSmoother diluSmoother_;
    GaussSeidelSmoother gsSmoother_;
public:
  //- Runtime type information
  TYPE_NAME("DILUGaussSeidel");
  // Constructors
    //- Construct from matrix components
    DILUGaussSeidelSmoother
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
  // Member Functions
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
