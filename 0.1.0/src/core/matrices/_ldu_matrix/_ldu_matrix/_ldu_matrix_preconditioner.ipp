// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_matrix.hpp"


template<class Type, class DType, class LUType>
mousse::autoPtr<typename mousse::LduMatrix<Type, DType, LUType>::preconditioner>
mousse::LduMatrix<Type, DType, LUType>::preconditioner::New
(
  const solver& sol,
  const dictionary& preconditionerDict
)
{
  word preconditionerName = preconditionerDict.lookup("preconditioner");
  if (sol.matrix().symmetric()) {
    typename symMatrixConstructorTable::iterator constructorIter =
      symMatrixConstructorTablePtr_->find(preconditionerName);
    if (constructorIter == symMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "LduMatrix<Type, DType, LUType>::preconditioner::New"
        "(const solver&, Istream&)",
        preconditionerDict
      )
      << "Unknown symmetric matrix preconditioner "
      << preconditionerName << endl << endl
      << "Valid symmetric matrix preconditioners are :" << endl
      << symMatrixConstructorTablePtr_->toc()
      << exit(FatalIOError);
    }
    return autoPtr<typename LduMatrix<Type, DType, LUType>::preconditioner>
    {
      constructorIter()
      (
        sol,
        preconditionerDict
      )
    };
  } else if (sol.matrix().asymmetric()) {
    typename asymMatrixConstructorTable::iterator constructorIter =
      asymMatrixConstructorTablePtr_->find(preconditionerName);
    if (constructorIter == asymMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "LduMatrix<Type, DType, LUType>::preconditioner::New"
        "(const solver&, Istream&)",
        preconditionerDict
      )
      << "Unknown asymmetric matrix preconditioner "
      << preconditionerName << endl << endl
      << "Valid asymmetric matrix preconditioners are :" << endl
      << asymMatrixConstructorTablePtr_->toc()
      << exit(FatalIOError);
    }
    return autoPtr<typename LduMatrix<Type, DType, LUType>::preconditioner>
    {
      constructorIter()
      (
        sol,
        preconditionerDict
      )
    };
  } else {
    FATAL_IO_ERROR_IN
    (
      "LduMatrix<Type, DType, LUType>::preconditioner::New"
      "(const solver&, Istream&)",
      preconditionerDict
    )
    << "cannot preconditione incomplete matrix, "
    "no diagonal or off-diagonal coefficient"
    << exit(FatalIOError);
    return autoPtr<typename LduMatrix<Type, DType, LUType>::preconditioner>
    {
      NULL
    };
  }
}
