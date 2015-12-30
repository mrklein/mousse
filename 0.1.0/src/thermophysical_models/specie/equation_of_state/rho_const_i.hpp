// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_const.hpp"
// Private Member Functions 
template<class Specie>
inline mousse::rhoConst<Specie>::rhoConst
(
  const Specie& sp,
  const scalar rho
)
:
  Specie(sp),
  rho_(rho)
{}
// Constructors 
template<class Specie>
inline mousse::rhoConst<Specie>::rhoConst
(
  const word& name,
  const rhoConst<Specie>& ico
)
:
  Specie(name, ico),
  rho_(ico.rho_)
{}
template<class Specie>
inline mousse::autoPtr<mousse::rhoConst<Specie> >
mousse::rhoConst<Specie>::clone() const
{
  return autoPtr<rhoConst<Specie> >(new rhoConst<Specie>(*this));
}
template<class Specie>
inline mousse::autoPtr<mousse::rhoConst<Specie> >
mousse::rhoConst<Specie>::New(Istream& is)
{
  return autoPtr<rhoConst<Specie> >(new rhoConst<Specie>(is));
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::rho(scalar p, scalar T) const
{
  return rho_;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::s(scalar p, scalar T) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::psi(scalar p, scalar T) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::Z(scalar p, scalar T) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::cpMcv(scalar p, scalar T) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::rhoConst<Specie>::operator+=(const rhoConst<Specie>& ico)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(ico);
  molr1 /= this->nMoles();
  scalar molr2 = ico.nMoles()/this->nMoles();
  rho_ = molr1*rho_ + molr2*ico.rho_;
}
template<class Specie>
inline void mousse::rhoConst<Specie>::operator-=(const rhoConst<Specie>& ico)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(ico);
  molr1 /= this->nMoles();
  scalar molr2 = ico.nMoles()/this->nMoles();
  rho_ = molr1*rho_ - molr2*ico.rho_;
}
template<class Specie>
inline void mousse::rhoConst<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator+
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  scalar nMoles = ico1.nMoles() + ico2.nMoles();
  scalar molr1 = ico1.nMoles()/nMoles;
  scalar molr2 = ico2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(ico1)
   + static_cast<const Specie&>(ico2),
    molr1*ico1.rho_ + molr2*ico2.rho_
  );
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator-
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  scalar nMoles = ico1.nMoles() + ico2.nMoles();
  scalar molr1 = ico1.nMoles()/nMoles;
  scalar molr2 = ico2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(ico1)
   - static_cast<const Specie&>(ico2),
    molr1*ico1.rho_ - molr2*ico2.rho_
  );
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator*
(
  const scalar s,
  const rhoConst<Specie>& ico
)
{
  return rhoConst<Specie>(s*static_cast<const Specie&>(ico), ico.rho_);
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator==
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  return ico2 - ico1;
}
