// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_phase_transport_model.hpp"
#include "viscosity_model.hpp"
#include "vol_fields.hpp"


// Constructors 
mousse::singlePhaseTransportModel::singlePhaseTransportModel
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  IOdictionary
  {
    {
      "transportProperties",
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  },
  viscosityModelPtr_{viscosityModel::New("nu", *this, U, phi)}
{}


// Destructor 
mousse::singlePhaseTransportModel::~singlePhaseTransportModel()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::singlePhaseTransportModel::nu() const
{
  return viscosityModelPtr_->nu();
}


mousse::tmp<mousse::scalarField>
mousse::singlePhaseTransportModel::nu(const label patchi) const
{
  return viscosityModelPtr_->nu(patchi);
}


void mousse::singlePhaseTransportModel::correct()
{
  viscosityModelPtr_->correct();
}


bool mousse::singlePhaseTransportModel::read()
{
  if (regIOobject::read()) {
    return viscosityModelPtr_->read(*this);
  } else {
    return false;
  }
}

