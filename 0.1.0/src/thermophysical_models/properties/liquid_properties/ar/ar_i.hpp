// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::Ar::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::Ar::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::Ar::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::Ar::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::Ar::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::Ar::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::Ar::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::Ar::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::Ar::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::Ar::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::Ar::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::Ar::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::Ar::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::Ar::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
