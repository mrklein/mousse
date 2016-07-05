// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "chemistry_solver.hpp"


// Constructors 
template<class ChemistryModel>
mousse::chemistrySolver<ChemistryModel>::chemistrySolver
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  ChemistryModel{mesh, phaseName}
{}


// Destructor 
template<class ChemistryModel>
mousse::chemistrySolver<ChemistryModel>::~chemistrySolver()
{}

