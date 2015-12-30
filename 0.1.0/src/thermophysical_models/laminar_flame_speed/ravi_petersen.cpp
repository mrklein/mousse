// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ravi_petersen.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace laminarFlameSpeedModels
{
  defineTypeNameAndDebug(RaviPetersen, 0);
  addToRunTimeSelectionTable
  (
    laminarFlameSpeed,
    RaviPetersen,
    dictionary
  );
}
}
// Constructors 
mousse::laminarFlameSpeedModels::RaviPetersen::RaviPetersen
(
  const dictionary& dict,
  const psiuReactionThermo& ct
)
:
  laminarFlameSpeed(dict, ct),
  coeffsDict_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
  pPoints_(coeffsDict_.lookup("pPoints")),
  EqRPoints_(coeffsDict_.lookup("EqRPoints")),
  alpha_(coeffsDict_.lookup("alpha")),
  beta_(coeffsDict_.lookup("beta")),
  TRef_(readScalar(coeffsDict_.lookup("TRef")))
{
  checkPointsMonotonicity("equivalenceRatio", EqRPoints_);
  checkPointsMonotonicity("pressure", pPoints_);
  checkCoefficientArrayShape("alpha", alpha_);
  checkCoefficientArrayShape("beta", beta_);
}
// Destructor 
mousse::laminarFlameSpeedModels::RaviPetersen::~RaviPetersen()
{}
// Private Member Functions 
void mousse::laminarFlameSpeedModels::RaviPetersen::checkPointsMonotonicity
(
  const word& name,
  const List<scalar>& x
) const
{
  for (label i = 1; i < x.size(); i ++)
  {
    if (x[i] <= x[i-1])
    {
      FatalIOErrorIn
      (
        "laminarFlameSpeedModels::RaviPetersen::checkPointsMonotonicity"
        "("
          "const word&, "
          "const List<scalar>&"
        ") const",
        coeffsDict_
      )   << "Data points for the " << name
        << " do not increase monotonically" << endl
        << exit(FatalIOError);
    }
  }
}
void mousse::laminarFlameSpeedModels::RaviPetersen::checkCoefficientArrayShape
(
  const word& name,
  const List<List<List<scalar> > >& x
) const
{
  bool ok = true;
  ok &= x.size() == EqRPoints_.size() - 1;
  forAll(x, i)
  {
    ok &= x[i].size() == pPoints_.size();
    forAll(x[i], j)
    {
      ok &= x[i][j].size() == x[i][0].size();
    }
  }
  if (!ok)
  {
    FatalIOErrorIn
    (
      "laminarFlameSpeedModels::RaviPetersen::checkCoefficientArrayShape"
      "("
        "const word&, "
        "const List<List<List<scalar> > >&"
      ") const",
      coeffsDict_
    )   << "Inconsistent size of " << name << " coefficients array" << endl
      << exit(FatalIOError);
  }
}
inline bool mousse::laminarFlameSpeedModels::RaviPetersen::interval
(
  const List<scalar>& xPoints,
  const scalar x,
  label& xIndex,
  scalar& xXi,
  scalar& xLim
) const
{
  if (x < xPoints.first())
  {
    xIndex = 0;
    xXi = 0.0;
    xLim = xPoints.first();
    return false;
  }
  else if (x > xPoints.last())
  {
    xIndex = xPoints.size() - 2;
    xXi = 1.0;
    xLim = xPoints.last();
    return false;
  }
  for (xIndex = 0; x > xPoints[xIndex+1]; xIndex ++)
  {
    // increment xIndex until xPoints[xIndex] < x < xPoints[xIndex+1]
  }
  xXi = (x - xPoints[xIndex])/(xPoints[xIndex+1] - xPoints[xIndex]);
  xLim = x;
  return true;
}
inline mousse::scalar mousse::laminarFlameSpeedModels::RaviPetersen::polynomial
(
  const List<scalar>& coeffs,
  const scalar x
) const
{
  scalar xPow = 1.0;
  scalar y = 0.0;
  forAll(coeffs, i)
  {
    y += coeffs[i]*xPow;
    xPow *= x;
  }
  return y;
}
inline mousse::scalar mousse::laminarFlameSpeedModels::RaviPetersen::dPolynomial
(
  const List<scalar>& coeffs,
  const scalar x
) const
{
  scalar xPow = 1.0;
  scalar y = 0.0;
  for (label i = 1; i < coeffs.size(); i++)
  {
    y += i*coeffs[i]*xPow;
    xPow *= x;
  }
  return y;
}
inline mousse::scalar mousse::laminarFlameSpeedModels::RaviPetersen::THatPowB
(
  const label EqRIndex,
  const label pIndex,
  const scalar EqR,
  const scalar Tu
) const
{
  return pow
  (
    Tu/TRef_,
    polynomial(beta_[EqRIndex][pIndex],EqR)
  );
}
inline mousse::scalar
mousse::laminarFlameSpeedModels::RaviPetersen::correlationInRange
(
  const label EqRIndex,
  const label pIndex,
  const scalar EqR,
  const scalar Tu
) const
{
  // standard correlation
  return
    polynomial(alpha_[EqRIndex][pIndex],EqR)
   *THatPowB(EqRIndex, pIndex, EqR, Tu);
}
inline mousse::scalar
mousse::laminarFlameSpeedModels::RaviPetersen::correlationOutOfRange
(
  const label EqRIndex,
  const label pIndex,
  const scalar EqR,
  const scalar EqRLim,
  const scalar Tu
) const
{
  scalar A = polynomial(alpha_[EqRIndex][pIndex], EqRLim);
  scalar dA = dPolynomial(alpha_[EqRIndex][pIndex], EqRLim);
  scalar dB = dPolynomial(beta_[EqRIndex][pIndex], EqRLim);
  scalar TB = THatPowB(EqRIndex, pIndex, EqRLim, Tu);
  // linear extrapolation from the bounds of the correlation
  return max(TB*(A + (dA + A*log(Tu/TRef_)*dB)*(EqR - EqRLim)), 0.0);
}
inline mousse::scalar mousse::laminarFlameSpeedModels::RaviPetersen::speed
(
  const scalar EqR,
  const scalar p,
  const scalar Tu
) const
{
  scalar Su = 0, s;
  label EqRIndex, pIndex;
  scalar EqRXi, pXi;
  scalar EqRLim, pLim;
  bool EqRInRange;
  EqRInRange = interval(EqRPoints_, EqR, EqRIndex, EqRXi, EqRLim);
  interval(pPoints_, p, pIndex, pXi, pLim);
  for (label pI = 0; pI < 2; pI ++)
  {
    if (EqRInRange)
    {
      s = correlationInRange(EqRIndex, pIndex + pI, EqR, Tu);
    }
    else
    {
      s = correlationOutOfRange(EqRIndex, pIndex + pI, EqR, EqRLim, Tu);
    }
    Su += (1 - pXi)*s;
    pXi = 1 - pXi;
  }
  return Su;
}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::laminarFlameSpeedModels::RaviPetersen::operator()() const
{
  const volScalarField& p = psiuReactionThermo_.p();
  const volScalarField& Tu = psiuReactionThermo_.Tu();
  volScalarField EqR
  (
    IOobject
    (
      "EqR",
      p.time().timeName(),
      p.db(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    p.mesh(),
    dimensionedScalar("EqR", dimless, 0.0)
  );
  if (psiuReactionThermo_.composition().contains("ft"))
  {
    const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");
    EqR =
      dimensionedScalar
      (
        psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
      )*ft/max(1 - ft, SMALL);
  }
  else
  {
    EqR = equivalenceRatio_;
  }
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
  forAll(Su0, cellI)
  {
    Su0[cellI] = speed(EqR[cellI], p[cellI], Tu[cellI]);
  }
  return tSu0;
}
