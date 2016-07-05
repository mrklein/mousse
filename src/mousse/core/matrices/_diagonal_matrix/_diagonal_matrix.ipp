// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_diagonal_matrix.hpp"


template<class Type>
inline mousse::DiagonalMatrix<Type>::DiagonalMatrix()
:
  List<Type>{}
{}


template<class Type>
template<class Form>
mousse::DiagonalMatrix<Type>::DiagonalMatrix(const Matrix<Form, Type>& a)
:
  List<Type>{min(a.n(), a.m())}
{
  FOR_ALL(*this, i) {
    this->operator[](i) = a[i][i];
  }
}


template<class Type>
mousse::DiagonalMatrix<Type>::DiagonalMatrix(const label size)
:
  List<Type>{size}
{}


template<class Type>
mousse::DiagonalMatrix<Type>::DiagonalMatrix(const label size, const Type& val)
:
  List<Type>{size, val}
{}


template<class Type>
mousse::DiagonalMatrix<Type>& mousse::DiagonalMatrix<Type>::invert()
{
  FOR_ALL(*this, i) {
    Type x = this->operator[](i);
    if (mag(x) < VSMALL) {
      this->operator[](i) = Type(0);
    } else {
      this->operator[](i) = Type(1)/x;
    }
  }
  return this;
}


template<class Type>
mousse::DiagonalMatrix<Type> mousse::inv(const DiagonalMatrix<Type>& A)
{
  DiagonalMatrix<Type> Ainv = A;
  FOR_ALL(A, i) {
    Type x = A[i];
    if (mag(x) < VSMALL) {
      Ainv[i] = Type(0);
    } else {
      Ainv[i] = Type(1)/x;
    }
  }
  return Ainv;
}
