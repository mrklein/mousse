// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gulders.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace laminarFlameSpeedModels
{
  defineTypeNameAndDebug(Gulders, 0);
  addToRunTimeSelectionTable
  (
    laminarFlameSpeed,
    Gulders,
    dictionary
  );
}
}
// Constructors 
mousse::laminarFlameSpeedModels::Gulders::Gulders
(
  const dictionary& dict,
  const psiuReactionThermo& ct
)
:
  laminarFlameSpeed(dict, ct),
  coeffsDict_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
  W_(readScalar(coeffsDict_.lookup("W"))),
  eta_(readScalar(coeffsDict_.lookup("eta"))),
  xi_(readScalar(coeffsDict_.lookup("xi"))),
  f_(readScalar(coeffsDict_.lookup("f"))),
  alpha_(readScalar(coeffsDict_.lookup("alpha"))),
  beta_(readScalar(coeffsDict_.lookup("beta")))
{}
// Destructor 
mousse::laminarFlameSpeedModels::Gulders::~Gulders()
{}
// Member Functions 
inline mousse::scalar mousse::laminarFlameSpeedModels::Gulders::SuRef
(
  scalar phi
) const
{
  if (phi > SMALL)
  {
    return W_*pow(phi, eta_)*exp(-xi_*sqr(phi - 1.075));
  }
  else
  {
    return 0.0;
  }
}
inline mousse::scalar mousse::laminarFlameSpeedModels::Gulders::Su0pTphi
(
  scalar p,
  scalar Tu,
  scalar phi,
  scalar Yres
) const
{
  static const scalar Tref = 300.0;
  static const scalar pRef = 1.013e5;
  return SuRef(phi)*pow((Tu/Tref), alpha_)*pow((p/pRef), beta_)*(1 - f_*Yres);
}
mousse::tmp<mousse::volScalarField> mousse::laminarFlameSpeedModels::Gulders::Su0pTphi
(
  const volScalarField& p,
  const volScalarField& Tu,
  scalar phi
) const
{
  tmp<volScalarField> tSu0
  (
    new volScalarField
    (
      IOobject
      (
        "Su0",
        p.time().timeName(),
        p.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      p.mesh(),
      dimensionedScalar("Su0", dimVelocity, 0.0)
    )
  );
  volScalarField& Su0 = tSu0();
  forAll(Su0, celli)
  {
    Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi, 0.0);
  }
  forAll(Su0.boundaryField(), patchi)
  {
    forAll(Su0.boundaryField()[patchi], facei)
    {
      Su0.boundaryField()[patchi][facei] =
        Su0pTphi
        (
          p.boundaryField()[patchi][facei],
          Tu.boundaryField()[patchi][facei],
          phi,
          0.0
        );
    }
  }
  return tSu0;
}
mousse::tmp<mousse::volScalarField> mousse::laminarFlameSpeedModels::Gulders::Su0pTphi
(
  const volScalarField& p,
  const volScalarField& Tu,
  const volScalarField& phi
) const
{
  tmp<volScalarField> tSu0
  (
    new volScalarField
    (
      IOobject
      (
        "Su0",
        p.time().timeName(),
        p.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      p.mesh(),
      dimensionedScalar("Su0", dimVelocity, 0.0)
    )
  );
  volScalarField& Su0 = tSu0();
  forAll(Su0, celli)
  {
    Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi[celli], 0.0);
  }
  forAll(Su0.boundaryField(), patchi)
  {
    forAll(Su0.boundaryField()[patchi], facei)
    {
      Su0.boundaryField()[patchi][facei] =
        Su0pTphi
        (
          p.boundaryField()[patchi][facei],
          Tu.boundaryField()[patchi][facei],
          phi.boundaryField()[patchi][facei],
          0.0
        );
    }
  }
  return tSu0;
}
mousse::tmp<mousse::volScalarField>
mousse::laminarFlameSpeedModels::Gulders::operator()() const
{
  if (psiuReactionThermo_.composition().contains("ft"))
  {
    const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");
    return Su0pTphi
    (
      psiuReactionThermo_.p(),
      psiuReactionThermo_.Tu(),
      dimensionedScalar
      (
        psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
      )*ft/max(1 - ft, SMALL)
    );
  }
  else
  {
    return Su0pTphi
    (
      psiuReactionThermo_.p(),
      psiuReactionThermo_.Tu(),
      equivalenceRatio_
    );
  }
}
