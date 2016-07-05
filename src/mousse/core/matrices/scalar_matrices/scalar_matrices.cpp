// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_matrices.hpp"
#include "svd.hpp"


// Member Functions 
void mousse::LUDecompose
(
  scalarSquareMatrix& matrix,
  labelList& pivotIndices
)
{
  label sign;
  LUDecompose(matrix, pivotIndices, sign);
}


void mousse::LUDecompose
(
  scalarSquareMatrix& matrix,
  labelList& pivotIndices,
  label& sign
)
{
  label n = matrix.n();
  scalar vv[n];
  sign = 1;
  for (label i=0; i<n; i++) {
    scalar largestCoeff = 0.0;
    scalar temp;
    const scalar* __restrict__ matrixi = matrix[i];
    for (label j=0; j<n; j++) {
      if ((temp = mag(matrixi[j])) > largestCoeff) {
        largestCoeff = temp;
      }
    }
    if (largestCoeff == 0.0) {
      FATAL_ERROR_IN
      (
        "LUdecompose"
        "(scalarSquareMatrix& matrix, labelList& rowIndices)"
      )
      << "Singular matrix" << exit(FatalError);
    }
    vv[i] = 1.0/largestCoeff;
  }
  for (label j=0; j<n; j++) {
    scalar* __restrict__ matrixj = matrix[j];
    for (label i=0; i<j; i++) {
      scalar* __restrict__ matrixi = matrix[i];
      scalar sum = matrixi[j];
      for (label k=0; k<i; k++) {
        sum -= matrixi[k]*matrix[k][j];
      }
      matrixi[j] = sum;
    }
    label iMax = 0;
    scalar largestCoeff = 0.0;
    for (label i=j; i<n; i++) {
      scalar* __restrict__ matrixi = matrix[i];
      scalar sum = matrixi[j];
      for (label k=0; k<j; k++) {
        sum -= matrixi[k]*matrix[k][j];
      }
      matrixi[j] = sum;
      scalar temp;
      if ((temp = vv[i]*mag(sum)) >= largestCoeff) {
        largestCoeff = temp;
        iMax = i;
      }
    }
    pivotIndices[j] = iMax;
    if (j != iMax) {
      scalar* __restrict__ matrixiMax = matrix[iMax];
      for (label k=0; k<n; k++) {
        Swap(matrixj[k], matrixiMax[k]);
      }
      sign *= -1;
      vv[iMax] = vv[j];
    }
    if (matrixj[j] == 0.0) {
      matrixj[j] = SMALL;
    }
    if (j != n-1) {
      scalar rDiag = 1.0/matrixj[j];
      for (label i=j+1; i<n; i++) {
        matrix[i][j] *= rDiag;
      }
    }
  }
}


void mousse::LUDecompose(scalarSymmetricSquareMatrix& matrix)
{
  // Store result in upper triangular part of matrix
  label size = matrix.n();
  // Set upper triangular parts to zero.
  for (label j = 0; j < size; j++) {
    for (label k = j + 1; k < size; k++) {
      matrix[j][k] = 0.0;
    }
  }
  for (label j = 0; j < size; j++) {
    scalar d = 0.0;
    for (label k = 0; k < j; k++) {
      scalar s = 0.0;
      for (label i = 0; i < k; i++) {
        s += matrix[i][k]*matrix[i][j];
      }
      s = (matrix[j][k] - s)/matrix[k][k];
      matrix[k][j] = s;
      matrix[j][k] = s;
      d += sqr(s);
    }
    d = matrix[j][j] - d;
    if (d < 0.0) {
      FATAL_ERROR_IN("mousse::LUDecompose(scalarSymmetricSquareMatrix&)")
        << "Matrix is not symmetric positive-definite. Unable to "
        << "decompose."
        << abort(FatalError);
    }
    matrix[j][j] = sqrt(d);
  }
}


// Global Functions 
void mousse::multiply
(
  scalarRectangularMatrix& ans,         // value changed in return
  const scalarRectangularMatrix& A,
  const scalarRectangularMatrix& B,
  const scalarRectangularMatrix& C
)
{
  if (A.m() != B.n()) {
    FATAL_ERROR_IN
    (
      "multiply("
      "const scalarRectangularMatrix& A, "
      "const scalarRectangularMatrix& B, "
      "const scalarRectangularMatrix& C, "
      "scalarRectangularMatrix& answer)"
    )
    << "A and B must have identical inner dimensions but A.m = "
    << A.m() << " and B.n = " << B.n()
    << abort(FatalError);
  }
  if (B.m() != C.n()) {
    FATAL_ERROR_IN
    (
      "multiply("
      "const scalarRectangularMatrix& A, "
      "const scalarRectangularMatrix& B, "
      "const scalarRectangularMatrix& C, "
      "scalarRectangularMatrix& answer)"
    )
    << "B and C must have identical inner dimensions but B.m = "
    << B.m() << " and C.n = " << C.n()
    << abort(FatalError);
  }
  ans = scalarRectangularMatrix(A.n(), C.m(), scalar(0));
  for (label i = 0; i < A.n(); i++) {
    for (label g = 0; g < C.m(); g++) {
      for (label l = 0; l < C.n(); l++) {
        scalar ab = 0;
        for (label j = 0; j < A.m(); j++) {
          ab += A[i][j]*B[j][l];
        }
        ans[i][g] += C[l][g] * ab;
      }
    }
  }
}


void mousse::multiply
(
  scalarRectangularMatrix& ans,         // value changed in return
  const scalarRectangularMatrix& A,
  const DiagonalMatrix<scalar>& B,
  const scalarRectangularMatrix& C
)
{
  if (A.m() != B.size()) {
    FATAL_ERROR_IN
    (
      "multiply("
      "const scalarRectangularMatrix& A, "
      "const DiagonalMatrix<scalar>& B, "
      "const scalarRectangularMatrix& C, "
      "scalarRectangularMatrix& answer)"
    )
    << "A and B must have identical inner dimensions but A.m = "
    << A.m() << " and B.n = " << B.size()
    << abort(FatalError);
  }
  if (B.size() != C.n()) {
    FATAL_ERROR_IN
    (
      "multiply("
      "const scalarRectangularMatrix& A, "
      "const DiagonalMatrix<scalar>& B, "
      "const scalarRectangularMatrix& C, "
      "scalarRectangularMatrix& answer)"
    )
    << "B and C must have identical inner dimensions but B.m = "
    << B.size() << " and C.n = " << C.n()
    << abort(FatalError);
  }
  ans = scalarRectangularMatrix{A.n(), C.m(), scalar(0)};
  for (label i = 0; i < A.n(); i++) {
    for (label g = 0; g < C.m(); g++) {
      for (label l = 0; l < C.n(); l++) {
        ans[i][g] += C[l][g] * A[i][l]*B[l];
      }
    }
  }
}


mousse::RectangularMatrix<mousse::scalar> mousse::SVDinv
(
  const scalarRectangularMatrix& A,
  scalar minCondition
)
{
  SVD svd{A, minCondition};
  return svd.VSinvUt();
}
