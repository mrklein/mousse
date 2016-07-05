// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_lubrication_model.hpp"
#include "phase_pair.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(wallLubricationModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(wallLubricationModel, dictionary);
}
const mousse::dimensionSet mousse::wallLubricationModel::dimF{1, -2, -2, 0, 0};
// Constructors 
mousse::wallLubricationModel::wallLubricationModel
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  wallDependentModel{pair.phase1().mesh()},
  pair_{pair}
{}
// Destructor 
mousse::wallLubricationModel::~wallLubricationModel()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField> mousse::wallLubricationModel::F() const
{
  return pair_.dispersed()*Fi();
}
mousse::tmp<mousse::surfaceScalarField> mousse::wallLubricationModel::Ff() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return fvc::interpolate(pair_.dispersed())*(fvc::interpolate(Fi()) & mesh.Sf());
}
