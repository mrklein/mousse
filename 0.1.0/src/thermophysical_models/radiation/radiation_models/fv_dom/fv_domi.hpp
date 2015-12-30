// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::radiation::radiativeIntensityRay&
mousse::radiation::fvDOM::IRay(const label rayI) const
{
  return  IRay_[rayI];
}
inline const mousse::volScalarField&
mousse::radiation::fvDOM::IRayLambda
(
  const label rayI,
  const label lambdaI
) const
{
  return IRay_[rayI].ILambda(lambdaI);
}
inline mousse::label mousse::radiation::fvDOM::nTheta() const
{
  return nTheta_;
}
inline mousse::label mousse::radiation::fvDOM::nPhi() const
{
  return nPhi_;
}
inline mousse::label mousse::radiation::fvDOM::nRay() const
{
  return nRay_;
}
inline mousse::label mousse::radiation::fvDOM::nLambda() const
{
  return nLambda_;
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::a() const
{
  return a_;
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::aLambda
(
  const label lambdaI
) const
{
  return aLambda_[lambdaI];
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::G() const
{
  return G_;
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::Qr() const
{
  return Qr_;
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::Qin() const
{
  return Qin_;
}
inline const mousse::volScalarField& mousse::radiation::fvDOM::Qem() const
{
  return Qem_;
}
inline const mousse::radiation::blackBodyEmission&
mousse::radiation::fvDOM::blackBody() const
{
  return blackBody_;
}
inline const mousse::fvScalarMatrix& mousse::radiation::fvDOM::fvRayDiv
(
  const label rayId,
  const label lambdaI
) const
{
  return fvRayDiv_[lambdaI][rayId];
}
inline bool mousse::radiation::fvDOM::cacheDiv() const
{
  return cacheDiv_;
}
inline mousse::scalar mousse::radiation::fvDOM::omegaMax() const
{
  return omegaMax_;
}
