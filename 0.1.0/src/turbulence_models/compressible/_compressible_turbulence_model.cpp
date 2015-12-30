// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_compressible_turbulence_model.hpp"
// Constructors 
template<class TransportModel>
mousse::CompressibleTurbulenceModel<TransportModel>::
CompressibleTurbulenceModel
(
  const word& type,
  const geometricOneField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  TurbulenceModel
  <
    geometricOneField,
    volScalarField,
    compressibleTurbulenceModel,
    transportModel
  >
  (
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  )
{}
// Selectors
template<class TransportModel>
mousse::autoPtr<mousse::CompressibleTurbulenceModel<TransportModel> >
mousse::CompressibleTurbulenceModel<TransportModel>::New
(
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  return autoPtr<CompressibleTurbulenceModel>
  (
    static_cast<CompressibleTurbulenceModel*>(
    TurbulenceModel
    <
      geometricOneField,
      volScalarField,
      compressibleTurbulenceModel,
      transportModel
    >::New
    (
      geometricOneField(),
      rho,
      U,
      phi,
      phi,
      transport,
      propertiesName
    ).ptr())
  );
}
