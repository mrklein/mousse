// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "consumption_speed.hpp"
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(consumptionSpeed, 0);
}
// Constructors 
mousse::consumptionSpeed::consumptionSpeed
(
  const dictionary& dict
)
:   omega0_(readScalar(dict.lookup("omega0"))),
  eta_(readScalar(dict.lookup("eta"))),
  sigmaExt_(readScalar(dict.lookup("sigmaExt"))),
  omegaMin_(readScalar(dict.lookup("omegaMin")))
{}
// Destructor 
mousse::consumptionSpeed::~consumptionSpeed()
{}
// Member Functions 
mousse::scalar mousse::consumptionSpeed::omega0Sigma
(
  scalar sigma,
  scalar a
) const
{
  scalar omega0 = 0.0;
  if (sigma < sigmaExt_)
  {
    omega0 = max
    (
      a*omega0_*(1.0 - exp(eta_*max(sigma, 0.0))),
      omegaMin_
    ) ;
  }
  return omega0;
}
mousse::tmp<mousse::volScalarField> mousse::consumptionSpeed::omega0Sigma
(
  const volScalarField& sigma
)
{
  tmp<volScalarField> tomega0
  (
    new volScalarField
    (
      IOobject
      (
        "omega0",
        sigma.time().timeName(),
        sigma.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      sigma.mesh(),
      dimensionedScalar
      (
        "omega0",
        dimensionSet(1, -2, -1, 0, 0, 0, 0),
        0
      )
    )
  );
  volScalarField& omega0 = tomega0();
  volScalarField::InternalField& iomega0 = omega0.internalField();
  FOR_ALL(iomega0, celli)
  {
    iomega0[celli] = omega0Sigma(sigma[celli], 1.0);
  }
  volScalarField::GeometricBoundaryField& bomega0 = omega0.boundaryField();
  FOR_ALL(bomega0, patchi)
  {
    FOR_ALL(bomega0[patchi], facei)
    {
      bomega0[patchi][facei] =
        omega0Sigma
        (
          sigma.boundaryField()[patchi][facei],
          1.0
        );
    }
  }
  return tomega0;
}
void  mousse::consumptionSpeed::read(const dictionary& dict)
{
  dict.lookup("omega0") >> omega0_ ;
  dict.lookup("eta") >> eta_ ;
  dict.lookup("sigmaExt") >> sigmaExt_;
  dict.lookup("omegaMin") >> omegaMin_;
}
