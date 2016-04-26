// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_chemistry_solver.hpp"
#include "add_to_run_time_selection_table.hpp"


// Constructors 
template<class ChemistryModel>
mousse::noChemistrySolver<ChemistryModel>::noChemistrySolver
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  chemistrySolver<ChemistryModel>{mesh, phaseName}
{}


// Destructor 
template<class ChemistryModel>
mousse::noChemistrySolver<ChemistryModel>::~noChemistrySolver()
{}


// Member Functions 
template<class ChemistryModel>
void mousse::noChemistrySolver<ChemistryModel>::solve
(
  scalarField&,
  scalar&,
  scalar&,
  scalar&,
  scalar&
) const
{}

