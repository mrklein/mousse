// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_matrix.hpp"


template<class Type, class DType, class LUType>
mousse::autoPtr<typename mousse::LduMatrix<Type, DType, LUType>::smoother>
mousse::LduMatrix<Type, DType, LUType>::smoother::New
(
  const word& fieldName,
  const LduMatrix<Type, DType, LUType>& matrix,
  const dictionary& smootherDict
)
{
  word smootherName = smootherDict.lookup("smoother");
  if (matrix.symmetric()) {
    typename symMatrixConstructorTable::iterator constructorIter =
      symMatrixConstructorTablePtr_->find(smootherName);
    if (constructorIter == symMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "LduMatrix<Type, DType, LUType>::smoother::New", smootherDict
      )
      << "Unknown symmetric matrix smoother " << smootherName
      << endl << endl
      << "Valid symmetric matrix smoothers are :" << endl
      << symMatrixConstructorTablePtr_->toc()
      << exit(FatalIOError);
    }
    return autoPtr<typename LduMatrix<Type, DType, LUType>::smoother>
    {
      constructorIter()
      (
        fieldName,
        matrix
      )
    };
  } else if (matrix.asymmetric()) {
    typename asymMatrixConstructorTable::iterator constructorIter =
      asymMatrixConstructorTablePtr_->find(smootherName);
    if (constructorIter == asymMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "LduMatrix<Type, DType, LUType>::smoother::New", smootherDict
      )
      << "Unknown asymmetric matrix smoother " << smootherName
      << endl << endl
      << "Valid asymmetric matrix smoothers are :" << endl
      << asymMatrixConstructorTablePtr_->toc()
      << exit(FatalIOError);
    }
    return autoPtr<typename LduMatrix<Type, DType, LUType>::smoother>
    {
      constructorIter()
      (
        fieldName,
        matrix
      )
    };
  } else {
    FATAL_IO_ERROR_IN
    (
      "LduMatrix<Type, DType, LUType>::smoother::New", smootherDict
    )
    << "cannot solve incomplete matrix, no off-diagonal coefficients"
    << exit(FatalIOError);
    return autoPtr<typename LduMatrix<Type, DType, LUType>::smoother>{NULL};
  }
}


// Constructors 
template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::smoother::smoother
(
  const word& fieldName,
  const LduMatrix<Type, DType, LUType>& matrix
)
:
  fieldName_{fieldName},
  matrix_{matrix}
{}
