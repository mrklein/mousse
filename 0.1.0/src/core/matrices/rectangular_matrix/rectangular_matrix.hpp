// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RectangularMatrix
// Description
//   A templated 2D rectangular m x n matrix of objects of \<Type\>.
//   The matrix dimensions are used for subscript bounds checking etc.
// SourceFiles
//   rectangular_matrix.cpp
#ifndef rectangular_matrix_hpp_
#define rectangular_matrix_hpp_
#include "matrix.hpp"
namespace mousse
{
template<class Type>
class RectangularMatrix
:
  public Matrix<RectangularMatrix<Type>, Type>
{
public:
  // Constructors
    //- Null constructor.
    inline RectangularMatrix();
    //- Construct given number of rows and columns,
    inline RectangularMatrix(const label m, const label n);
    //- Construct with given number of rows and columns
    //  and value for all elements.
    inline RectangularMatrix(const label m, const label n, const Type&);
    //- Construct from Istream.
    inline RectangularMatrix(Istream&);
    //- Clone
    inline autoPtr<RectangularMatrix<Type>> clone() const;
};
}  // namespace mousse

// Constructors 
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix()
:
  Matrix<RectangularMatrix<Type>, Type>{}
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix
(
  const label m,
  const label n
)
:
  Matrix<RectangularMatrix<Type>, Type>{m, n}
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix
(
  const label m,
  const label n,
  const Type& t
)
:
  Matrix<RectangularMatrix<Type>, Type>{m, n, t}
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix(Istream& is)
:
  Matrix<RectangularMatrix<Type>, Type>{is}
{}
template<class Type>
inline mousse::autoPtr<mousse::RectangularMatrix<Type>>
mousse::RectangularMatrix<Type>::clone() const
{
  return {new RectangularMatrix<Type>(*this)};
}
#endif
