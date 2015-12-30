// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_absorption_emission.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    defineTypeNameAndDebug(noAbsorptionEmission, 0);
    addToRunTimeSelectionTable
    (
      absorptionEmissionModel,
      noAbsorptionEmission,
      dictionary
    );
  }
}
// Constructors 
mousse::radiation::noAbsorptionEmission::noAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel(dict, mesh)
{}
// Destructor 
mousse::radiation::noAbsorptionEmission::~noAbsorptionEmission()
{}
