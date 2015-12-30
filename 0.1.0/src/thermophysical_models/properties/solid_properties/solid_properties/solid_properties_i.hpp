// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
inline mousse::scalar mousse::solidProperties::rho() const
{
  return rho_;
}
inline mousse::scalar mousse::solidProperties::Cp() const
{
  return Cp_;
}
inline mousse::scalar mousse::solidProperties::K() const
{
  return K_;
}
inline mousse::scalar mousse::solidProperties::Hf() const
{
  return Hf_;
}
inline mousse::scalar mousse::solidProperties::Hs(const scalar T) const
{
  return Cp_*(T - Tstd);
}
inline mousse::scalar mousse::solidProperties::emissivity() const
{
  return emissivity_;
}
