// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compressible_turbulence_model.hpp"
#include "surface_interpolate.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(compressibleTurbulenceModel, 0);
}
// Constructors 
mousse::compressibleTurbulenceModel::compressibleTurbulenceModel
(
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const word& propertiesName
)
:
  turbulenceModel
  (
    U,
    alphaRhoPhi,
    phi,
    propertiesName
  ),
  rho_(rho)
{}
// Member Functions 
mousse::tmp<mousse::surfaceScalarField>
mousse::compressibleTurbulenceModel::phi() const
{
  if (phi_.dimensions() == dimensionSet(0, 3, -1, 0, 0))
  {
    return phi_;
  }
  else
  {
    return phi_/fvc::interpolate(rho_);
  }
}
void mousse::compressibleTurbulenceModel::correctEnergyTransport()
{}
