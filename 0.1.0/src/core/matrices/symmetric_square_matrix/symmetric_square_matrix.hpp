#ifndef CORE_MATRICES_SYMMETRIC_SQUARE_MATRIX_SYMMETRIC_SQUARE_MATRIX_HPP_
#define CORE_MATRICES_SYMMETRIC_SQUARE_MATRIX_SYMMETRIC_SQUARE_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SymmetricSquareMatrix
// Description
//   A templated 2D square symmetric matrix of objects of \<T\>, where the
//   n x n matrix dimension is known and used for subscript bounds checking, etc.
// SourceFiles
//   symmetric_square_matrix.cpp
#include "square_matrix.hpp"
namespace mousse
{
template<class Type>
class SymmetricSquareMatrix
:
  public Matrix<SymmetricSquareMatrix<Type>, Type>
{
public:
  // Constructors
    //- Null constructor.
    inline SymmetricSquareMatrix();
    //- Construct given number of rows/columns.
    inline SymmetricSquareMatrix(const label n);
    //- Construct with given number of rows/columns
    inline SymmetricSquareMatrix(const label m, const label n);
    //- Construct with given number of rows/columns
    //  and value for all elements.
    inline SymmetricSquareMatrix(const label m, const label n, const Type&);
    //- Construct from Istream.
    inline SymmetricSquareMatrix(Istream&);
    //- Clone
    inline autoPtr<SymmetricSquareMatrix<Type> > clone() const;
    //- Return subscript-checked row of Matrix.
    inline Type& operator()(const label r, const label c);
    //- Return subscript-checked row of constant Matrix.
    inline const Type& operator()(const label r, const label c) const;
};
// Global functions
//- Return the LU decomposed SymmetricSquareMatrix inverse
template<class Type>
SymmetricSquareMatrix<Type> invDecomposed(const SymmetricSquareMatrix<Type>&);
//- Return the SymmetricSquareMatrix inverse
template<class Type>
SymmetricSquareMatrix<Type> inv(const SymmetricSquareMatrix<Type>&);
//- Return the LU decomposed SymmetricSquareMatrix det
template<class Type>
scalar detDecomposed(const SymmetricSquareMatrix<Type>&);
//- Return the SymmetricSquareMatrix det
template<class Type>
scalar det(const SymmetricSquareMatrix<Type>&);
}  // namespace mousse

// Constructors 
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix()
:
  Matrix<SymmetricSquareMatrix<Type>, Type>{}
{}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix(const label n)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>{n, n}
{}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix
(
  const label m,
  const label n
)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>{m, n}
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SymmetricSquareMatrix<Type>::SymmetricSquareMatrix"
      "(const label m, const label n)"
    )
    << "m != n for constructing a symmetric square matrix"
    << exit(FatalError);
  }
}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix
(
  const label m,
  const label n,
  const Type& t
)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>{m, n, t}
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SymmetricSquareMatrix<Type>::SymmetricSquareMatrix"
      "(const label m, const label n, const Type&)"
    )
    << "m != n for constructing a symmetric square matrix"
    << exit(FatalError);
  }
}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix(Istream& is)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>{is}
{}
template<class Type>
inline mousse::autoPtr<mousse::SymmetricSquareMatrix<Type> >
mousse::SymmetricSquareMatrix<Type>::clone() const
{
  return {new SymmetricSquareMatrix<Type>(*this)};
}
template<class Type>
inline Type& mousse::SymmetricSquareMatrix<Type>::operator()
(
  const label r,
  const label c
)
{
  return (r > c) ? this->operator[](r)[c] : this->operator[](c)[r];
}
template<class Type>
inline const Type& mousse::SymmetricSquareMatrix<Type>::operator()
(
  const label r,
  const label c
) const
{
  return (r > c) ? this->operator[](r)[c] : this->operator[](c)[r];
}
#ifdef NoRepository
#   include "symmetric_square_matrix.cpp"
#endif
#endif
