// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_perfect_gas.hpp"
// Constructors 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const Specie& sp,  const scalar pRef
)
:
  Specie(sp),
  pRef_(pRef)
{}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const incompressiblePerfectGas& ipg
)
:
  Specie(ipg),
  pRef_(ipg.pRef_)
{}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const word& name,
  const incompressiblePerfectGas<Specie>& ipg
)
:
  Specie(name, ipg),
  pRef_(ipg.pRef_)
{}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::clone() const
{
  return autoPtr<incompressiblePerfectGas<Specie> >
  (
    new incompressiblePerfectGas<Specie>(*this)
  );
}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::New
(
  Istream& is
)
{
  return autoPtr<incompressiblePerfectGas<Specie> >
  (
    new incompressiblePerfectGas<Specie>(is)
  );
}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::New
(
  const dictionary& dict
)
{
  return autoPtr<incompressiblePerfectGas<Specie> >
  (
    new incompressiblePerfectGas<Specie>(dict)
  );
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::rho
(
  scalar /*p*/,
  scalar T
) const
{
  return pRef_/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::s
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::psi
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::Z
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::cpMcv
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return RR;
}
// Member Operators 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>&
mousse::incompressiblePerfectGas<Specie>::operator=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  Specie::operator=(ipg);
  pRef_ = ipg.pRef_;
  return *this;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator+=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(ipg);
  molr1 /= this->nMoles();
  scalar molr2 = ipg.nMoles()/this->nMoles();
  pRef_ = molr1*pRef_ + molr2*ipg.pRef_;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator-=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  Specie::operator-=(ipg);
  pRef_ = ipg.pRef_;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator+
(
  const incompressiblePerfectGas<Specie>& ipg1,
  const incompressiblePerfectGas<Specie>& ipg2
)
{
  scalar nMoles = ipg1.nMoles() + ipg2.nMoles();
  scalar molr1 = ipg1.nMoles()/nMoles;
  scalar molr2 = ipg2.nMoles()/nMoles;
  return incompressiblePerfectGas<Specie>
  (
    static_cast<const Specie&>(ipg1)
   + static_cast<const Specie&>(ipg2),
    molr1*ipg1.pRef_ + molr2*ipg2.pRef_
  );
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator-
(
  const incompressiblePerfectGas<Specie>& ipg1,
  const incompressiblePerfectGas<Specie>& ipg2
)
{
  return incompressiblePerfectGas<Specie>
  (
    static_cast<const Specie&>(ipg1)
   - static_cast<const Specie&>(ipg2),
    ipg1.pRef_
  );
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator*
(
  const scalar s,
  const incompressiblePerfectGas<Specie>& ipg
)
{
  return incompressiblePerfectGas<Specie>
  (
    s*static_cast<const Specie&>(ipg),
    ipg.pRef_
  );
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator==
(
  const incompressiblePerfectGas<Specie>& pg1,
  const incompressiblePerfectGas<Specie>& pg2
)
{
  return pg2 - pg1;
}
