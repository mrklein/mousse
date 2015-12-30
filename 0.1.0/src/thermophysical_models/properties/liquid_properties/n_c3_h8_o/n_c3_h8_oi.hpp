// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::nC3H8O::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::nC3H8O::D(scalar p, scalar T, scalar) const
{
  // Use 'standard' NSRDS D(p, T) function
  return D(p, T);
}
