// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::H2O::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::H2O::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::H2O::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::H2O::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::H2O::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::H2O::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::H2O::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::H2O::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::H2O::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::H2O::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::H2O::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::H2O::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::H2O::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::H2O::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
