// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DICGaussSeidelSmoother
// Description
//   Combined DIC/GaussSeidel smoother for symmetric matrices in which DIC
//   smoothing is followed by GaussSeidel to ensure that any "spikes" created
//   by the DIC sweeps are smoothed-out.
// SourceFiles
//   dic_gauss_seidel_smoother.cpp
#ifndef dic_gauss_seidel_smoother_hpp_
#define dic_gauss_seidel_smoother_hpp_
#include "dic_smoother.hpp"
#include "gauss_seidel_smoother.hpp"
namespace mousse
{
class DICGaussSeidelSmoother
:
  public lduMatrix::smoother
{
  // Private data
    DICSmoother dicSmoother_;
    GaussSeidelSmoother gsSmoother_;
public:
  //- Runtime type information
  TYPE_NAME("DICGaussSeidel");
  // Constructors
    //- Construct from matrix components
    DICGaussSeidelSmoother
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
