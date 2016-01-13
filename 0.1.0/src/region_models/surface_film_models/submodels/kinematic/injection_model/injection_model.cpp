// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "injection_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(injectionModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(injectionModel, dictionary);
// Protected Member Functions 
void injectionModel::addToInjectedMass(const scalar dMass)
{
  injectedMass_ += dMass;
}
// Constructors 
injectionModel::injectionModel(surfaceFilmModel& owner)
:
  filmSubModelBase(owner),
  injectedMass_(0.0)
{}
injectionModel::injectionModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase(owner, dict, typeName, modelType),
  injectedMass_(0.0)
{}
// Destructor 
injectionModel::~injectionModel()
{}
// Member Functions 
void injectionModel::correct()
{
  if (outputTime())
  {
    scalar injectedMass0 = getModelProperty<scalar>("injectedMass");
    injectedMass0 += returnReduce(injectedMass_, sumOp<scalar>());
    setModelProperty<scalar>("injectedMass", injectedMass0);
    injectedMass_ = 0.0;
  }
}
scalar injectionModel::injectedMassTotal() const
{
  scalar injectedMass0 = getModelProperty<scalar>("injectedMass");
  return injectedMass0 + returnReduce(injectedMass_, sumOp<scalar>());
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
