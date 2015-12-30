// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "perfect_gas.hpp"
// Private Member Functions 
template<class Specie>
inline mousse::perfectGas<Specie>::perfectGas(const Specie& sp)
:
  Specie(sp)
{}
// Constructors 
template<class Specie>
inline mousse::perfectGas<Specie>::perfectGas
(
  const word& name,
  const perfectGas<Specie>& pg
)
:
  Specie(name, pg)
{}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie> >
mousse::perfectGas<Specie>::clone() const
{
  return autoPtr<perfectGas<Specie> >(new perfectGas<Specie>(*this));
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie> >
mousse::perfectGas<Specie>::New(Istream& is)
{
  return autoPtr<perfectGas<Specie> >(new perfectGas<Specie>(is));
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie> > mousse::perfectGas<Specie>::New
(
  const dictionary& dict
)
{
  return autoPtr<perfectGas<Specie> >(new perfectGas<Specie>(dict));
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::rho(scalar p, scalar T) const
{
  return p/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::s(scalar p, scalar T) const
{
  return -RR*log(p/Pstd);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::psi(scalar p, scalar T) const
{
  return 1.0/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::Z(scalar p, scalar T) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::cpMcv(scalar p, scalar T) const
{
  return RR;
}
// Member Operators 
template<class Specie>
inline void mousse::perfectGas<Specie>::operator+=(const perfectGas<Specie>& pg)
{
  Specie::operator+=(pg);
}
template<class Specie>
inline void mousse::perfectGas<Specie>::operator-=(const perfectGas<Specie>& pg)
{
  Specie::operator-=(pg);
}
template<class Specie>
inline void mousse::perfectGas<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator+
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return perfectGas<Specie>
  (
    static_cast<const Specie&>(pg1)
   + static_cast<const Specie&>(pg2)
  );
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator-
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return perfectGas<Specie>
  (
    static_cast<const Specie&>(pg1)
   - static_cast<const Specie&>(pg2)
  );
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator*
(
  const scalar s,
  const perfectGas<Specie>& pg
)
{
  return perfectGas<Specie>(s*static_cast<const Specie&>(pg));
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator==
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return pg2 - pg1;
}
