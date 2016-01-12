// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_incompressible_turbulence_model.hpp"
// Constructors 
template<class TransportModel>
mousse::IncompressibleTurbulenceModel<TransportModel>::
IncompressibleTurbulenceModel
(
  const word& /*type*/,
  const geometricOneField& alpha,
  const geometricOneField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const TransportModel& transport,
  const word& propertiesName
)
:
  TurbulenceModel
  <
    geometricOneField,
    geometricOneField,
    incompressibleTurbulenceModel,
    TransportModel
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
mousse::autoPtr<mousse::IncompressibleTurbulenceModel<TransportModel> >
mousse::IncompressibleTurbulenceModel<TransportModel>::New
(
  const volVectorField& U,
  const surfaceScalarField& phi,
  const TransportModel& transport,
  const word& propertiesName
)
{
  return autoPtr<IncompressibleTurbulenceModel>
  (
    static_cast<IncompressibleTurbulenceModel*>(
    TurbulenceModel
    <
      geometricOneField,
      geometricOneField,
      incompressibleTurbulenceModel,
      TransportModel
    >::New
    (
      geometricOneField(),
      geometricOneField(),
      U,
      phi,
      phi,
      transport,
      propertiesName
    ).ptr())
  );
}
// Member Functions 
template<class TransportModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::IncompressibleTurbulenceModel<TransportModel>::devReff() const
{
  return devRhoReff();
}
template<class TransportModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::IncompressibleTurbulenceModel<TransportModel>::divDevReff
(
  volVectorField& U
) const
{
  return divDevRhoReff(U);
}
template<class TransportModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::IncompressibleTurbulenceModel<TransportModel>::
devRhoReff() const
{
  NOT_IMPLEMENTED
  (
    "IncompressibleTurbulenceModel<TransportModel>::"
    "devRhoReff()"
  );
  return devReff();
}
template<class TransportModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::IncompressibleTurbulenceModel<TransportModel>::
divDevRhoReff
(
  volVectorField& U
) const
{
  NOT_IMPLEMENTED
  (
    "IncompressibleTurbulenceModel<TransportModel>::"
    "divDevRhoReff(volVectorField& U)"
  );
  return divDevReff(U);
}
template<class TransportModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::IncompressibleTurbulenceModel<TransportModel>::
divDevRhoReff
(
  const volScalarField& /*rho*/,
  volVectorField& U
) const
{
  NOT_IMPLEMENTED
  (
    "IncompressibleTurbulenceModel<TransportModel>::"
    "divDevRhoReff(const volScalarField& rho, volVectorField& U)"
  );
  return divDevReff(U);
}
