// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeName(noDecomp);
  addNamedToRunTimeSelectionTable
  (
    decompositionMethod,
    noDecomp,
    dictionary,
    none
  );
}
// Constructors 
mousse::noDecomp::noDecomp(const dictionary& decompositionDict)
:
  decompositionMethod(decompositionDict)
{}
