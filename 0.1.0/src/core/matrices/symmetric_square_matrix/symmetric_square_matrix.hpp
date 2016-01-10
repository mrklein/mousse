// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SymmetricSquareMatrix
// Description
//   A templated 2D square symmetric matrix of objects of \<T\>, where the
//   n x n matrix dimension is known and used for subscript bounds checking, etc.
// SourceFiles
//   symmetric_square_matrix_i.hpp
//   symmetric_square_matrix.cpp
#ifndef symmetric_square_matrix_hpp_
#define symmetric_square_matrix_hpp_
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
#   include "symmetric_square_matrix_i.hpp"
#ifdef NoRepository
#   include "symmetric_square_matrix.cpp"
#endif
#endif
