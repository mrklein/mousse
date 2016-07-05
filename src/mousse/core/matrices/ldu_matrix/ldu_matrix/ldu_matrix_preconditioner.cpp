// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


// Static Data Members
namespace mousse {

DEFINE_RUN_TIME_SELECTION_TABLE(lduMatrix::preconditioner, symMatrix);
DEFINE_RUN_TIME_SELECTION_TABLE(lduMatrix::preconditioner, asymMatrix);

}


mousse::word mousse::lduMatrix::preconditioner::getName
(
  const dictionary& solverControls
)
{
  word name;
  // handle primitive or dictionary entry
  const entry& e = solverControls.lookupEntry("preconditioner", false, false);
  if (e.isDict()) {
    e.dict().lookup("preconditioner") >> name;
  } else {
    e.stream() >> name;
  }
  return name;
}


mousse::autoPtr<mousse::lduMatrix::preconditioner>
mousse::lduMatrix::preconditioner::New
(
  const solver& sol,
  const dictionary& solverControls
)
{
  word name;
  // handle primitive or dictionary entry
  const entry& e = solverControls.lookupEntry("preconditioner", false, false);
  if (e.isDict()) {
    e.dict().lookup("preconditioner") >> name;
  } else {
    e.stream() >> name;
  }
  const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;
  if (sol.matrix().symmetric()) {
    symMatrixConstructorTable::iterator constructorIter =
      symMatrixConstructorTablePtr_->find(name);
    if (constructorIter == symMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "lduMatrix::preconditioner::New"
        "(const solver&, const dictionary&)",
        controls
      )
      << "Unknown symmetric matrix preconditioner "
      << name << nl << nl
      << "Valid symmetric matrix preconditioners :" << endl
      << symMatrixConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
    }
    return
      autoPtr<lduMatrix::preconditioner>
      {
        constructorIter()
        (
          sol,
          controls
        )
      };
  } else if (sol.matrix().asymmetric()) {
    asymMatrixConstructorTable::iterator constructorIter =
      asymMatrixConstructorTablePtr_->find(name);
    if (constructorIter == asymMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "lduMatrix::preconditioner::New"
        "(const solver&, const dictionary&)",
        controls
      )
      << "Unknown asymmetric matrix preconditioner "
      << name << nl << nl
      << "Valid asymmetric matrix preconditioners :" << endl
      << asymMatrixConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
    }
    return
      autoPtr<lduMatrix::preconditioner>
      {
        constructorIter()
        (
          sol,
          controls
        )
      };
  } else {
    FATAL_IO_ERROR_IN
    (
      "lduMatrix::preconditioner::New"
      "(const solver&, const dictionary&)",
      controls
    )
    << "cannot solve incomplete matrix, "
       "no diagonal or off-diagonal coefficient"
    << exit(FatalIOError);
    return autoPtr<lduMatrix::preconditioner>{nullptr};
  }
}
