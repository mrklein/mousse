#ifndef CORE_MATRICES_SQUARE_MATRIX_SQUARE_MATRIX_HPP_
#define CORE_MATRICES_SQUARE_MATRIX_SQUARE_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SquareMatrix
// Description
//   A templated 2D square matrix of objects of \<T\>, where the n x n matrix
//   dimension is known and used for subscript bounds checking, etc.
// SourceFiles
//   square_matrix.cpp
#include "matrix.hpp"
namespace mousse
{
template<class Type>
class SquareMatrix
:
  public Matrix<SquareMatrix<Type>, Type>
{
public:
  // Constructors
    //- Null constructor.
    inline SquareMatrix();
    //- Construct given number of rows/columns.
    inline SquareMatrix(const label n);
    //- Construct given number of rows and columns,
    //  It checks that m == n.
    inline SquareMatrix(const label m, const label n);
    //- Construct with given number of rows and rows
    //  and value for all elements.
    //  It checks that m == n.
    inline SquareMatrix(const label m, const label n, const Type&);
    //- Construct from Istream.
    inline SquareMatrix(Istream&);
    //- Clone
    inline autoPtr<SquareMatrix<Type>> clone() const;
};
// Global functions
//- Return the LU decomposed SquareMatrix det
template<class Type>
scalar detDecomposed(const SquareMatrix<Type>&, const label sign);
//- Return the SquareMatrix det
template<class Type>
scalar det(const SquareMatrix<Type>&);
//- Return the SquareMatrix det and the LU decomposition in the original matrix
template<class Type>
scalar det(SquareMatrix<Type>&);
}  // namespace mousse

// Constructors 
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix()
:
  Matrix<SquareMatrix<Type>, Type>{}
{}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix(const label n)
:
  Matrix<SquareMatrix<Type>, Type>{n, n}
{}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix(const label m, const label n)
:
  Matrix<SquareMatrix<Type>, Type>{m, n}
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SquareMatrix<Type>::SquareMatrix(const label m, const label n)"
    ) << "m != n for constructing a square matrix" << exit(FatalError);
  }
}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix
(
  const label m,
  const label n,
  const Type& t
)
:
  Matrix<SquareMatrix<Type>, Type>{m, n, t}
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SquareMatrix<Type>::SquareMatrix"
      "(const label m, const label n, const Type&)"
    ) << "m != n for constructing a square matrix" << exit(FatalError);
  }
}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix(Istream& is)
:
  Matrix<SquareMatrix<Type>, Type>{is}
{}
template<class Type>
inline mousse::autoPtr<mousse::SquareMatrix<Type>>
mousse::SquareMatrix<Type>::clone() const
{
  return {new SquareMatrix<Type>(*this)};
}
#ifdef NoRepository
#   include "square_matrix.cpp"
#endif
#endif
