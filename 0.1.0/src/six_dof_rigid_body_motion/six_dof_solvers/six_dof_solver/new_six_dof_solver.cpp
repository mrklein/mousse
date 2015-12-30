// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_solver.hpp"
// Selector 
mousse::autoPtr<mousse::sixDoFSolver> mousse::sixDoFSolver::New
(
  const dictionary& dict,
  sixDoFRigidBodyMotion& body
)
{
  word sixDoFSolverType(dict.lookup("type"));
  Info<< "Selecting sixDoFSolver " << sixDoFSolverType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(sixDoFSolverType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn("sixDoFSolver::New")
      << "Unknown sixDoFSolverType type "
      << sixDoFSolverType << endl << endl
      << "Valid sixDoFSolver types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, body);
}
