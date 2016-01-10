// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ode_solver.hpp"
mousse::autoPtr<mousse::ODESolver> mousse::ODESolver::New
(
  const ODESystem& odes,
  const dictionary& dict
)
{
  word ODESolverTypeName(dict.lookup("solver"));
  Info<< "Selecting ODE solver " << ODESolverTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(ODESolverTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "ODESolver::New"
      "(const dictionary& dict, const ODESystem& odes)"
    )   << "Unknown ODESolver type "
      << ODESolverTypeName << nl << nl
      << "Valid ODESolvers are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<ODESolver>(cstrIter()(odes, dict));
}
