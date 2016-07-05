// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_matrices.hpp"
#include "swap.hpp"
#include "list_ops.hpp"


// Member Functions 
template<class Type>
void mousse::solve
(
  scalarSquareMatrix& tmpMatrix,
  List<Type>& sourceSol
)
{
  label n = tmpMatrix.n();
  // Elimination
  for (label i=0; i<n; i++) {
    label iMax = i;
    scalar largestCoeff = mag(tmpMatrix[iMax][i]);
    // Swap entries around to find a good pivot
    for (label j=i+1; j<n; j++) {
      if (mag(tmpMatrix[j][i]) > largestCoeff) {
        iMax = j;
        largestCoeff = mag(tmpMatrix[iMax][i]);
      }
    }
    if (i != iMax) {
      for (label k=i; k<n; k++) {
        Swap(tmpMatrix[i][k], tmpMatrix[iMax][k]);
      }
      Swap(sourceSol[i], sourceSol[iMax]);
    }
    // Check that the system of equations isn't singular
    if (mag(tmpMatrix[i][i]) < 1e-20) {
      FATAL_ERROR_IN("solve(scalarSquareMatrix&, Field<Type>& sourceSol)")
        << "Singular Matrix"
        << exit(FatalError);
    }
    // Reduce to upper triangular form
    for (label j=i+1; j<n; j++) {
      sourceSol[j] -= sourceSol[i]*(tmpMatrix[j][i]/tmpMatrix[i][i]);
      for (label k=n-1; k>=i; k--) {
        tmpMatrix[j][k] -=
          tmpMatrix[i][k]*tmpMatrix[j][i]/tmpMatrix[i][i];
      }
    }
  }
  // Back-substitution
  for (label j=n-1; j>=0; j--) {
    Type ntempvec = pTraits<Type>::zero;
    for (label k=j+1; k<n; k++) {
      ntempvec += tmpMatrix[j][k]*sourceSol[k];
    }
    sourceSol[j] = (sourceSol[j] - ntempvec)/tmpMatrix[j][j];
  }
}


template<class Type>
void mousse::solve
(
  List<Type>& psi,
  const scalarSquareMatrix& matrix,
  const List<Type>& source
)
{
  scalarSquareMatrix tmpMatrix = matrix;
  psi = source;
  solve(tmpMatrix, psi);
}


template<class Type>
void mousse::LUBacksubstitute
(
  const scalarSquareMatrix& luMatrix,
  const labelList& pivotIndices,
  List<Type>& sourceSol
)
{
  label n = luMatrix.n();
  label ii = 0;
  for (label i=0; i<n; i++) {
    label ip = pivotIndices[i];
    Type sum = sourceSol[ip];
    sourceSol[ip] = sourceSol[i];
    const scalar* __restrict__ luMatrixi = luMatrix[i];
    if (ii != 0) {
      for (label j=ii-1; j<i; j++) {
        sum -= luMatrixi[j]*sourceSol[j];
      }
    } else if (sum != pTraits<Type>::zero) {
      ii = i+1;
    }
    sourceSol[i] = sum;
  }
  for (label i=n-1; i>=0; i--) {
    Type sum = sourceSol[i];
    const scalar* __restrict__ luMatrixi = luMatrix[i];
    for (label j=i+1; j<n; j++) {
      sum -= luMatrixi[j]*sourceSol[j];
    }
    sourceSol[i] = sum/luMatrixi[i];
  }
}


template<class Type>
void mousse::LUBacksubstitute
(
  const scalarSymmetricSquareMatrix& luMatrix,
  List<Type>& sourceSol
)
{
  label n = luMatrix.n();
  label ii = 0;
  for (label i=0; i<n; i++) {
    Type sum = sourceSol[i];
    const scalar* __restrict__ luMatrixi = luMatrix[i];
    if (ii != 0) {
      for (label j=ii-1; j<i; j++) {
        sum -= luMatrixi[j]*sourceSol[j];
      }
    } else if (sum != pTraits<Type>::zero) {
      ii = i+1;
    }
    sourceSol[i] = sum/luMatrixi[i];
  }
  for (label i=n-1; i>=0; i--) {
    Type sum = sourceSol[i];
    const scalar* __restrict__ luMatrixi = luMatrix[i];
    for (label j=i+1; j<n; j++) {
      sum -= luMatrixi[j]*sourceSol[j];
    }
    sourceSol[i] = sum/luMatrixi[i];
  }
}


template<class Type>
void mousse::LUsolve
(
  scalarSquareMatrix& matrix,
  List<Type>& sourceSol
)
{
  labelList pivotIndices(matrix.n());
  LUDecompose(matrix, pivotIndices);
  LUBacksubstitute(matrix, pivotIndices, sourceSol);
}


template<class Type>
void mousse::LUsolve
(
  scalarSymmetricSquareMatrix& matrix,
  List<Type>& sourceSol
)
{
  LUDecompose(matrix);
  LUBacksubstitute(matrix, sourceSol);
}


template<class Form, class Type>
void mousse::multiply
(
  Matrix<Form, Type>& ans,         // value changed in return
  const Matrix<Form, Type>& A,
  const Matrix<Form, Type>& B
)
{
  if (A.m() != B.n()) {
    FATAL_ERROR_IN
    (
      "multiply("
      "Matrix<Form, Type>& answer "
      "const Matrix<Form, Type>& A, "
      "const Matrix<Form, Type>& B)"
    )
    << "A and B must have identical inner dimensions but A.m = "
    << A.m() << " and B.n = " << B.n()
    << abort(FatalError);
  }
  ans = Matrix<Form, Type>(A.n(), B.m(), scalar(0));
  for (label i = 0; i < A.n(); i++) {
    for (label j = 0; j < B.m(); j++) {
      for (label l = 0; l < B.n(); l++) {
        ans[i][j] += A[i][l]*B[l][j];
      }
    }
  }
}
