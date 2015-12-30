// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::CH3OH::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::CH3OH::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
