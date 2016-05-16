// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase.hpp"


// Constructors 
mousse::phase::phase
(
  const word& phaseName,
  const dictionary& phaseDict,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  volScalarField
  {
    {
      IOobject::groupName("alpha", phaseName),
      U.mesh().time().timeName(),
      U.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    U.mesh()
  },
  name_{phaseName},
  phaseDict_{phaseDict},
  nuModel_
  {
    viscosityModel::New
    (
      IOobject::groupName("nu", phaseName),
      phaseDict_,
      U,
      phi
    )
  },
  rho_{"rho", dimDensity, phaseDict_}
{}


// Member Functions 
mousse::autoPtr<mousse::phase> mousse::phase::clone() const
{
  NOT_IMPLEMENTED("phase::clone() const");
  return autoPtr<phase>{nullptr};
}


void mousse::phase::correct()
{
  nuModel_->correct();
}


bool mousse::phase::read(const dictionary& phaseDict)
{
  phaseDict_ = phaseDict;
  if (nuModel_->read(phaseDict_)) {
    phaseDict_.lookup("rho") >> rho_;
    return true;
  }
  return false;
}
