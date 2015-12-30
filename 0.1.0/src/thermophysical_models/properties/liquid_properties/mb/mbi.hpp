// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::MB::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::MB::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::MB::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::MB::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::MB::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::MB::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::MB::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::MB::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::MB::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::MB::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::MB::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::MB::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::MB::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::MB::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
