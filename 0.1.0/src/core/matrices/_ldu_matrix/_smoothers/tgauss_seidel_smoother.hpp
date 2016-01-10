// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TGaussSeidelSmoother
// Description
//   mousse::TGaussSeidelSmoother
// SourceFiles
//   tgauss_seidel_smoother.cpp

#ifndef tgauss_seidel_smoother_hpp_
#define tgauss_seidel_smoother_hpp_

#include "_ldu_matrix.hpp"

namespace mousse
{
template<class Type, class DType, class LUType>
class TGaussSeidelSmoother
:
  public LduMatrix<Type, DType, LUType>::smoother
{
  // Private data
    //- The inverse (reciprocal for scalars) diagonal
    Field<DType> rD_;
public:
  //- Runtime type information
  TYPE_NAME("GaussSeidel");
  // Constructors
    //- Construct from components
    TGaussSeidelSmoother
    (
      const word& fieldName,
      const LduMatrix<Type, DType, LUType>& matrix
    );
  // Member Functions
    //- Smooth for the given number of sweeps
    static void smooth
    (
      const word& fieldName,
      Field<Type>& psi,
      const LduMatrix<Type, DType, LUType>& matrix,
      const Field<DType>& rD,
      const label nSweeps
    );
    //- Smooth the solution for a given number of sweeps
    virtual void smooth
    (
      Field<Type>& psi,
      const label nSweeps
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "tgauss_seidel_smoother.cpp"
#endif
#endif
