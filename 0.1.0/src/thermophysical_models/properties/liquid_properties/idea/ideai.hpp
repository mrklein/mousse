// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::IDEA::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::IDEA::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::IDEA::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::IDEA::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::IDEA::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::IDEA::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::IDEA::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::IDEA::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::IDEA::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::IDEA::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::IDEA::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::IDEA::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::IDEA::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::IDEA::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
