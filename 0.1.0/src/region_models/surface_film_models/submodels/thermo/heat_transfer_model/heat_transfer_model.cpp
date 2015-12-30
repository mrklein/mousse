// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(heatTransferModel, 0);
defineRunTimeSelectionTable(heatTransferModel, dictionary);
// Constructors 
heatTransferModel::heatTransferModel
(
  surfaceFilmModel& owner
)
:
  filmSubModelBase(owner)
{}
heatTransferModel::heatTransferModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase(owner, dict, typeName, modelType)
{}
// Destructor 
heatTransferModel::~heatTransferModel()
{}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
