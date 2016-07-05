// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_dispersion_model.hpp"
#include "phase_pair.hpp"
#include "fvc_grad.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(turbulentDispersionModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(turbulentDispersionModel, dictionary);
}
const mousse::dimensionSet mousse::turbulentDispersionModel::dimD{1, -1, -2, 0, 0};
const mousse::dimensionSet mousse::turbulentDispersionModel::dimF{1, -2, -2, 0, 0};
// Constructors 
mousse::turbulentDispersionModel::turbulentDispersionModel
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  pair_{pair}
{}
// Destructor 
mousse::turbulentDispersionModel::~turbulentDispersionModel()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField>
mousse::turbulentDispersionModel::F() const
{
  return D()*fvc::grad(pair_.dispersed());
}
