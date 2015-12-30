// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::C12H26::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::C12H26::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::C12H26::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::C12H26::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::C12H26::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::C12H26::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::C12H26::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::C12H26::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::C12H26::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::C12H26::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::C12H26::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::C12H26::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::C12H26::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::C12H26::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
