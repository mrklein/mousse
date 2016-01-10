// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix()
:
  Matrix<RectangularMatrix<Type>, Type>()
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix
(
  const label m,
  const label n
)
:
  Matrix<RectangularMatrix<Type>, Type>(m, n)
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix
(
  const label m,
  const label n,
  const Type& t
)
:
  Matrix<RectangularMatrix<Type>, Type>(m, n, t)
{}
template<class Type>
inline mousse::RectangularMatrix<Type>::RectangularMatrix(Istream& is)
:
  Matrix<RectangularMatrix<Type>, Type>(is)
{}
template<class Type>
inline mousse::autoPtr<mousse::RectangularMatrix<Type> >
mousse::RectangularMatrix<Type>::clone() const
{
  return autoPtr<RectangularMatrix<Type> >
  (
    new RectangularMatrix<Type>(*this)
  );
}
