// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetric_square_matrix.hpp"
template<class Type>
mousse::SymmetricSquareMatrix<Type> mousse::invDecomposed
(
  const SymmetricSquareMatrix<Type>& matrix
)
{
  SymmetricSquareMatrix<Type> inv(matrix.n(), matrix.n(), 0.0);
  for (label i = 0; i < matrix.n(); ++i)
  {
    inv[i][i] = 1.0/matrix[i][i];
    for (label j = 0; j < i; ++j)
    {
      scalar sum = 0.0;
      for (label k = j; k < i; k++)
      {
        sum -= matrix[i][k]*inv[k][j];
      }
      inv[i][j] = sum/matrix[i][i];
    }
  }
  return inv.T()*inv;
}
template<class Type>
mousse::SymmetricSquareMatrix<Type> mousse::inv
(
  const SymmetricSquareMatrix<Type>& matrix
)
{
  SymmetricSquareMatrix<Type> matrixTmp(matrix);
  LUDecompose(matrixTmp);
  return invDecomposed(matrixTmp);
}
template<class Type>
mousse::scalar mousse::detDecomposed(const SymmetricSquareMatrix<Type>& matrix)
{
  scalar diagProduct = 1.0;
  for (label i = 0; i < matrix.n(); ++i)
  {
    diagProduct *= matrix[i][i];
  }
  return sqr(diagProduct);
}
template<class Type>
mousse::scalar mousse::det(const SymmetricSquareMatrix<Type>& matrix)
{
  SymmetricSquareMatrix<Type> matrixTmp = matrix;
  LUDecompose(matrixTmp);
  return detDecomposed(matrixTmp);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
