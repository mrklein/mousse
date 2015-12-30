// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline bool mousse::radiation::absorptionCoeffs::invTemp() const
{
  return  invTemp_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Tcommon() const
{
  return  Tcommon_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Tlow() const
{
  return  Tlow_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Thigh() const
{
  return  Thigh_;
}
inline const mousse::radiation::absorptionCoeffs::coeffArray&
mousse::radiation::absorptionCoeffs::highACoeffs() const
{
  return  highACoeffs_;
}
inline const mousse::radiation::absorptionCoeffs::coeffArray&
mousse::radiation::absorptionCoeffs::lowACoeffs() const
{
  return  lowACoeffs_;
}
