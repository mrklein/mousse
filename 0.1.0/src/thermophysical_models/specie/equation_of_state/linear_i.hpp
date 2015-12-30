// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear.hpp"
// Private Member Functions 
template<class Specie>
inline mousse::linear<Specie>::linear
(
  const Specie& sp,
  const scalar psi,
  const scalar rho0
)
:
  Specie(sp),
  psi_(psi),
  rho0_(rho0)
{}
// Constructors 
template<class Specie>
inline mousse::linear<Specie>::linear
(
  const word& name,
  const linear<Specie>& pf
)
:
  Specie(name, pf),
  psi_(pf.psi_),
  rho0_(pf.rho0_)
{}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::clone() const
{
  return autoPtr<linear<Specie> >(new linear<Specie>(*this));
}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::New(Istream& is)
{
  return autoPtr<linear<Specie> >(new linear<Specie>(is));
}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::New
(
  const dictionary& dict
)
{
  return autoPtr<linear<Specie> >(new linear<Specie>(dict));
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::rho(scalar p, scalar T) const
{
  return rho0_ + psi_*p;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::s(scalar p, scalar T) const
{
  return -log((rho0_ + psi_*p)/(rho0_ + psi_*Pstd))/(T*psi_);
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::psi(scalar p, scalar T) const
{
  return psi_;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::Z(scalar p, scalar T) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::cpMcv(scalar p, scalar T) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::linear<Specie>::operator+=
(
  const linear<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  psi_ = molr1*psi_ + molr2*pf.psi_;
  rho0_ = molr1*rho0_ + molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::linear<Specie>::operator-=
(
  const linear<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  psi_ = molr1*psi_ - molr2*pf.psi_;
  rho0_ = molr1*rho0_ - molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::linear<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::linear<Specie> mousse::operator+
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   + static_cast<const Specie&>(pf2),
    molr1*pf1.psi_ + molr2*pf2.psi_,
    molr1*pf1.rho0_ + molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator-
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   - static_cast<const Specie&>(pf2),
    molr1*pf1.psi_ - molr2*pf2.psi_,
    molr1*pf1.rho0_ - molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator*
(
  const scalar s,
  const linear<Specie>& pf
)
{
  return linear<Specie>
  (
    s*static_cast<const Specie&>(pf),
    pf.psi_,
    pf.rho0_
  );
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator==
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  return pf2 - pf1;
}
