// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_change_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(phaseChangeModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(phaseChangeModel, dictionary);
// Constructors 
phaseChangeModel::phaseChangeModel
(
  surfaceFilmModel& owner
)
:
  filmSubModelBase(owner),
  latestMassPC_(0.0),
  totalMassPC_(0.0)
{}
phaseChangeModel::phaseChangeModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase(owner, dict, typeName, modelType),
  latestMassPC_(0.0),
  totalMassPC_(0.0)
{}
// Destructor 
phaseChangeModel::~phaseChangeModel()
{}
// Member Functions 
void phaseChangeModel::correct
(
  const scalar dt,
  scalarField& availableMass,
  volScalarField& dMass,
  volScalarField& dEnergy
)
{
  if (!active())
  {
    return;
  }
  correctModel
  (
    dt,
    availableMass,
    dMass,
    dEnergy
  );
  latestMassPC_ = sum(dMass.internalField());
  totalMassPC_ += latestMassPC_;
  availableMass -= dMass;
  dMass.correctBoundaryConditions();
  if (outputTime())
  {
    scalar phaseChangeMass = getModelProperty<scalar>("phaseChangeMass");
    phaseChangeMass += returnReduce(totalMassPC_, sumOp<scalar>());
    setModelProperty<scalar>("phaseChangeMass", phaseChangeMass);
    totalMassPC_ = 0.0;
  }
}
void phaseChangeModel::info(Ostream& os) const
{
  const scalar massPCRate =
    returnReduce(latestMassPC_, sumOp<scalar>())
   /owner_.time().deltaTValue();
  scalar phaseChangeMass = getModelProperty<scalar>("phaseChangeMass");
  phaseChangeMass += returnReduce(totalMassPC_, sumOp<scalar>());
  os  << indent << "mass phase change  = " << phaseChangeMass << nl
    << indent << "vapourisation rate = " << massPCRate << nl;
}
} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace mousse
