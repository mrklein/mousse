#ifndef CORE_MATRICES_SCALAR_MATRICES_SVD_SVD_HPP_
#define CORE_MATRICES_SCALAR_MATRICES_SVD_SVD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SVD
// Description
//   Singular value decomposition of a rectangular matrix.
// SourceFiles
//   svdi.hpp
//   svd.cpp


#include "scalar_matrices.hpp"

namespace mousse
{

class SVD
{
  // Private data

    //- Rectangular matrix with the same dimensions as the input
    scalarRectangularMatrix U_;

    //- Square matrix V
    scalarRectangularMatrix V_;

    //- The singular values
    DiagonalMatrix<scalar> S_;

    //- The matrix product V S^(-1) U^T
    scalarRectangularMatrix VSinvUt_;

    //- The number of zero singular values
    label nZeros_;

  // Private Member Functions
    template<class T>
    inline const T sign(const T& a, const T& b);

public:

  // Constructors

    //- Construct from a rectangular Matrix
    SVD(const scalarRectangularMatrix& A, const scalar minCondition = 0);

    //- Disallow default bitwise copy construct
    SVD(const SVD&) = delete;

    //- Disallow default bitwise assignment
    SVD& operator=(const SVD&) = delete;

  // Access functions

    //- Return U
    inline const scalarRectangularMatrix& U() const;

    //- Return the square matrix V
    inline const scalarRectangularMatrix& V() const;

    //- Return the singular values
    inline const scalarDiagonalMatrix& S() const;

    //- Return VSinvUt (the pseudo inverse)
    inline const scalarRectangularMatrix& VSinvUt() const;

    //- Return the number of zero singular values
    inline label nZeros() const;

    //- Return the minimum non-zero singular value
    inline scalar minNonZeroS() const;
};
}  // namespace mousse

// Private Member Functions 
template<class T>
inline const T mousse::SVD::sign(const T& a, const T& b)
{
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

// Member Functions 
inline const mousse::scalarRectangularMatrix& mousse::SVD::U() const
{
  return U_;
}

inline const mousse::scalarRectangularMatrix& mousse::SVD::V() const
{
  return V_;
}

inline const mousse::scalarDiagonalMatrix& mousse::SVD::S() const
{
  return S_;
}

inline const mousse::scalarRectangularMatrix& mousse::SVD::VSinvUt() const
{
  return VSinvUt_;
}

inline mousse::label mousse::SVD::nZeros() const
{
  return nZeros_;
}

inline mousse::scalar mousse::SVD::minNonZeroS() const
{
  scalar minS = S_[0];
  for (label i = 1; i < S_.size(); i++)
  {
    scalar s = S_[i];
    if (s > VSMALL && s < minS) minS = s;
  }
  return minS;
}

#endif
