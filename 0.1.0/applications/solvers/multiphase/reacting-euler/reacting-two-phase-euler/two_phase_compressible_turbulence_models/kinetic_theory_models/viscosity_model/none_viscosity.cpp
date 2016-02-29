// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "none_viscosity.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(noneViscosity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(viscosityModel, noneViscosity, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::noneViscosity::noneViscosity(const dictionary& dict)
:
  viscosityModel(dict)
{}
// Destructor 
mousse::kineticTheoryModels::noneViscosity::~noneViscosity()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::kineticTheoryModels::noneViscosity::nu
(
  const volScalarField& alpha1,
  const volScalarField& /*Theta*/,
  const volScalarField& /*g0*/,
  const volScalarField& /*rho1*/,
  const volScalarField& /*da*/,
  const dimensionedScalar& /*e*/
) const
{
  return dimensionedScalar{"0", {0, 2, -1, 0, 0, 0, 0}, 0.0}*alpha1;
}
