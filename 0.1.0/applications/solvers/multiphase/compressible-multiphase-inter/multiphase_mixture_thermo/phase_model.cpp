// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_model.hpp"
// Constructors 
mousse::phaseModel::phaseModel
(
  const word& phaseName,
  const volScalarField& p,
  const volScalarField& T
)
:
  volScalarField
  {
    {
      IOobject::groupName("alpha", phaseName),
      p.mesh().time().timeName(),
      p.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    p.mesh()
  },
  name_{phaseName},
  p_{p},
  T_{T},
  thermo_{NULL},
  dgdt_
  {
    {
      IOobject::groupName("dgdt", phaseName),
      p.mesh().time().timeName(),
      p.mesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    p.mesh(),
    {"0", dimless/dimTime, 0}
  }
{
  {
    volScalarField Tp{IOobject::groupName("T", phaseName), T};
    Tp.write();
  }
  thermo_ = rhoThermo::New(p.mesh(), phaseName);
  thermo_->validate(phaseName, "e");
  correct();
}
// Member Functions 
mousse::autoPtr<mousse::phaseModel> mousse::phaseModel::clone() const
{
  NOT_IMPLEMENTED("phaseModel::clone() const");
  return autoPtr<phaseModel>{NULL};
}
void mousse::phaseModel::correct()
{
  thermo_->he() = thermo_->he(p_, T_);
  thermo_->correct();
}
