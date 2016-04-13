// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "square_matrix.hpp"
#include "label_list.hpp"


template<class Type>
mousse::scalar mousse::detDecomposed
(
  const SquareMatrix<Type>& matrix,
  const label sign
)
{
  scalar diagProduct = 1.0;
  for (label i = 0; i < matrix.n(); ++i) {
    diagProduct *= matrix[i][i];
  }
  return sign*diagProduct;
}


template<class Type>
mousse::scalar mousse::det(const SquareMatrix<Type>& matrix)
{
  SquareMatrix<Type> matrixTmp = matrix;
  labelList pivotIndices{matrix.n()};
  label sign;
  LUDecompose(matrixTmp, pivotIndices, sign);
  return detDecomposed(matrixTmp, sign);
}


template<class Type>
mousse::scalar mousse::det(SquareMatrix<Type>& matrix)
{
  labelList pivotIndices{matrix.n()};
  label sign;
  LUDecompose(matrix, pivotIndices, sign);
  return detDecomposed(matrix, sign);
}

