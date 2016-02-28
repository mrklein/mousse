// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "virtual_mass_model.hpp"
#include "phase_pair.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(virtualMassModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(virtualMassModel, dictionary);
}
const mousse::dimensionSet mousse::virtualMassModel::dimK(dimDensity);
// Constructors 
mousse::virtualMassModel::virtualMassModel
(
  const dictionary& /*dict*/,
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
// Destructor 
mousse::virtualMassModel::~virtualMassModel()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::virtualMassModel::Ki() const
{
  return Cvm()*pair_.continuous().rho();
}
mousse::tmp<mousse::volScalarField> mousse::virtualMassModel::K() const
{
  return pair_.dispersed()*Ki();
}
mousse::tmp<mousse::surfaceScalarField> mousse::virtualMassModel::Kf() const
{
  return
    fvc::interpolate(pair_.dispersed())*fvc::interpolate(Ki());
}
bool mousse::virtualMassModel::writeData(Ostream& os) const
{
  return os.good();
}
