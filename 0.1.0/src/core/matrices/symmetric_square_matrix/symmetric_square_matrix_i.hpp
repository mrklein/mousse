// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix()
:
  Matrix<SymmetricSquareMatrix<Type>, Type>()
{}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix(const label n)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>(n, n)
{}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix
(
  const label m,
  const label n
)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>(m, n)
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SymmetricSquareMatrix<Type>::SymmetricSquareMatrix"
      "(const label m, const label n)"
    )   << "m != n for constructing a symmetric square matrix"
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
  Matrix<SymmetricSquareMatrix<Type>, Type>(m, n, t)
{
  if (m != n)
  {
    FATAL_ERROR_IN
    (
      "SymmetricSquareMatrix<Type>::SymmetricSquareMatrix"
      "(const label m, const label n, const Type&)"
    )   << "m != n for constructing a symmetric square matrix"
      << exit(FatalError);
  }
}
template<class Type>
inline mousse::SymmetricSquareMatrix<Type>::SymmetricSquareMatrix(Istream& is)
:
  Matrix<SymmetricSquareMatrix<Type>, Type>(is)
{}
template<class Type>
inline mousse::autoPtr<mousse::SymmetricSquareMatrix<Type> >
mousse::SymmetricSquareMatrix<Type>::clone() const
{
  return autoPtr<SymmetricSquareMatrix<Type> >
  (
    new SymmetricSquareMatrix<Type>(*this)
  );
}
template<class Type>
inline Type& mousse::SymmetricSquareMatrix<Type>::operator()
(
  const label r,
  const label c
)
{
  if (r > c)
  {
    return this->operator[](r)[c];
  }
  else
  {
    return this->operator[](c)[r];
  }
}
template<class Type>
inline const Type& mousse::SymmetricSquareMatrix<Type>::operator()
(
  const label r,
  const label c
) const
{
  if (r > c)
  {
    return this->operator[](r)[c];
  }
  else
  {
    return this->operator[](c)[r];
  }
}
