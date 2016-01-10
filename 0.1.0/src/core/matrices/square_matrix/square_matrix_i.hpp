// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix()
:
  Matrix<SquareMatrix<Type>, Type>()
{}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix(const label n)
:
  Matrix<SquareMatrix<Type>, Type>(n, n)
{}
template<class Type>
inline mousse::SquareMatrix<Type>::SquareMatrix(const label m, const label n)
:
  Matrix<SquareMatrix<Type>, Type>(m, n)
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
  Matrix<SquareMatrix<Type>, Type>(m, n, t)
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
  Matrix<SquareMatrix<Type>, Type>(is)
{}
template<class Type>
inline mousse::autoPtr<mousse::SquareMatrix<Type> >
mousse::SquareMatrix<Type>::clone() const
{
  return autoPtr<SquareMatrix<Type> >(new SquareMatrix<Type>(*this));
}
