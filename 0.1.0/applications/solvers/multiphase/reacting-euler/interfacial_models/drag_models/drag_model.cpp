// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "drag_model.hpp"
#include "phase_pair.hpp"
#include "swarm_correction.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(dragModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(dragModel, dictionary);
}
const mousse::dimensionSet mousse::dragModel::dimK(1, -3, -1, 0, 0);
// Constructors 
mousse::dragModel::dragModel
(
  const phasePair& pair,
  const bool registerObject
)
:
  regIOobject
  {
    {
      IOobject::groupName(typeName, pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      registerObject
    }
  },
  pair_{pair}
{}
mousse::dragModel::dragModel
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  regIOobject
  {
    {
      IOobject::groupName(typeName, pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      registerObject
    }
  },
  pair_{pair},
  swarmCorrection_
  {
    swarmCorrection::New
    (
      dict.subDict("swarmCorrection"),
      pair
    )
  }
{}
// Destructor 
mousse::dragModel::~dragModel()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModel::Ki() const
{
  return
    0.75
    *CdRe()
    *swarmCorrection_->Cs()
    *pair_.continuous().rho()
    *pair_.continuous().nu()
    /sqr(pair_.dispersed().d());
}
mousse::tmp<mousse::volScalarField> mousse::dragModel::K() const
{
  return max(pair_.dispersed(), pair_.dispersed().residualAlpha())*Ki();
}
mousse::tmp<mousse::surfaceScalarField> mousse::dragModel::Kf() const
{
  return
    max(fvc::interpolate(pair_.dispersed()),
        pair_.dispersed().residualAlpha())*fvc::interpolate(Ki());
}
bool mousse::dragModel::writeData(Ostream& os) const
{
  return os.good();
}
