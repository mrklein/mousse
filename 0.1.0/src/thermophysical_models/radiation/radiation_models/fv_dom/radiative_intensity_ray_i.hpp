// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::volScalarField&
mousse::radiation::radiativeIntensityRay::I() const
{
  return I_;
}
inline const mousse::volScalarField&
mousse::radiation::radiativeIntensityRay::Qr() const
{
  return Qr_;
}
inline mousse::volScalarField& mousse::radiation::radiativeIntensityRay::Qr()
{
  return Qr_;
}
inline const mousse::volScalarField& mousse::radiation::
radiativeIntensityRay::Qin() const
{
  return Qin_;
}
inline mousse::volScalarField& mousse::radiation::radiativeIntensityRay::Qin()
{
  return Qin_;
}
inline const mousse::volScalarField& mousse::radiation::
radiativeIntensityRay::Qem() const
{
  return Qem_;
}
inline mousse::volScalarField& mousse::radiation::radiativeIntensityRay::Qem()
{
  return Qem_;
}
inline const mousse::vector& mousse::radiation::radiativeIntensityRay::d() const
{
  return d_;
}
inline const mousse::vector& mousse::radiation::radiativeIntensityRay::dAve() const
{
  return dAve_;
}
inline mousse::scalar mousse::radiation::radiativeIntensityRay::nLambda() const
{
  return nLambda_;
}
inline mousse::scalar mousse::radiation::radiativeIntensityRay::phi() const
{
  return phi_;
}
inline mousse::scalar mousse::radiation::radiativeIntensityRay::theta() const
{
  return theta_;
}
inline mousse::scalar mousse::radiation::radiativeIntensityRay::omega() const
{
  return omega_;
}
inline const mousse::volScalarField&
mousse::radiation::radiativeIntensityRay::ILambda
(
  const label lambdaI
) const
{
  return ILambda_[lambdaI];
}
