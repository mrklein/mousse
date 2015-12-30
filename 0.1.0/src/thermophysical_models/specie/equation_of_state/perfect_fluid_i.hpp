// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "perfect_fluid.hpp"
#include "specie.hpp"
// Private Member Functions 
template<class Specie>
inline mousse::perfectFluid<Specie>::perfectFluid
(
  const Specie& sp,
  const scalar R,
  const scalar rho0
)
:
  Specie(sp),
  R_(R),
  rho0_(rho0)
{}
// Constructors 
template<class Specie>
inline mousse::perfectFluid<Specie>::perfectFluid
(
  const word& name,
  const perfectFluid<Specie>& pf
)
:
  Specie(name, pf),
  R_(pf.R_),
  rho0_(pf.rho0_)
{}
template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::clone() const
{
  return autoPtr<perfectFluid<Specie> >(new perfectFluid<Specie>(*this));
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::New(Istream& is)
{
  return autoPtr<perfectFluid<Specie> >(new perfectFluid<Specie>(is));
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::New
(
  const dictionary& dict
)
{
  return autoPtr<perfectFluid<Specie> >(new perfectFluid<Specie>(dict));
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::R() const
{
  return R_;
}
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::rho(scalar p, scalar T) const
{
  return rho0_ + p/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::s(scalar p, scalar T) const
{
  return -RR*log(p/Pstd);
}
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::psi(scalar p, scalar T) const
{
  return 1.0/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::Z(scalar p, scalar T) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::cpMcv(scalar p, scalar T) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::perfectFluid<Specie>::operator+=
(
  const perfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  R_ = 1.0/(molr1/R_ + molr2/pf.R_);
  rho0_ = molr1*rho0_ + molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::perfectFluid<Specie>::operator-=
(
  const perfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  R_ = 1.0/(molr1/R_ - molr2/pf.R_);
  rho0_ = molr1*rho0_ - molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::perfectFluid<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator+
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return perfectFluid<Specie>
  (
    static_cast<const Specie&>(pf1)
   + static_cast<const Specie&>(pf2),
    1.0/(molr1/pf1.R_ + molr2/pf2.R_),
    molr1*pf1.rho0_ + molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator-
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return perfectFluid<Specie>
  (
    static_cast<const Specie&>(pf1)
   - static_cast<const Specie&>(pf2),
    1.0/(molr1/pf1.R_ - molr2/pf2.R_),
    molr1*pf1.rho0_ - molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator*
(
  const scalar s,
  const perfectFluid<Specie>& pf
)
{
  return perfectFluid<Specie>
  (
    s*static_cast<const Specie&>(pf),
    pf.R_,
    pf.rho0_
  );
}
template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator==
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  return pf2 - pf1;
}
