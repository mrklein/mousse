// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixture_viscosity_model.hpp"
#include "vol_fields.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(mixtureViscosityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(mixtureViscosityModel, dictionary);

}


// Constructors 
mousse::mixtureViscosityModel::mixtureViscosityModel
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  name_{name},
  viscosityProperties_{viscosityProperties},
  U_{U},
  phi_{phi}
{}


// Member Functions 
bool mousse::mixtureViscosityModel::read(const dictionary& viscosityProperties)
{
  viscosityProperties_ = viscosityProperties;
  return true;
}

