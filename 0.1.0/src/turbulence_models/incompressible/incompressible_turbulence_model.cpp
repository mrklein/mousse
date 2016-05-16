// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_turbulence_model.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(incompressibleTurbulenceModel, 0);

}


// Constructors 
mousse::incompressibleTurbulenceModel::incompressibleTurbulenceModel
(
  const geometricOneField&,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const word& propertiesName
)
:
  turbulenceModel
  {
    U,
    alphaRhoPhi,
    phi,
    propertiesName
  }
{}


mousse::tmp<mousse::volScalarField>
mousse::incompressibleTurbulenceModel::mu() const
{
  return nu();
}


mousse::tmp<mousse::scalarField>
mousse::incompressibleTurbulenceModel::mu(const label patchi) const
{
  return nu(patchi);
}


mousse::tmp<mousse::volScalarField>
mousse::incompressibleTurbulenceModel::mut() const
{
  return nut();
}


mousse::tmp<mousse::scalarField>
mousse::incompressibleTurbulenceModel::mut(const label patchi) const
{
  return nut(patchi);
}


mousse::tmp<mousse::volScalarField>
mousse::incompressibleTurbulenceModel::muEff() const
{
  return nuEff();
}


mousse::tmp<mousse::scalarField>
mousse::incompressibleTurbulenceModel::muEff(const label patchi) const
{
  return nuEff(patchi);
}

