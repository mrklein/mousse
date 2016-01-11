// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
// Constructors 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const polynomialTransport& pt
)
:
  Thermo(pt),
  muCoeffs_(pt.muCoeffs_),
  kappaCoeffs_(pt.kappaCoeffs_)
{}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const Thermo& t,
  const Polynomial<PolySize>& muCoeffs,
  const Polynomial<PolySize>& kappaCoeffs
)
:
  Thermo(t),
  muCoeffs_(muCoeffs),
  kappaCoeffs_(kappaCoeffs)
{}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const word& name,
  const polynomialTransport& pt
)
:
  Thermo(name, pt),
  muCoeffs_(pt.muCoeffs_),
  kappaCoeffs_(pt.kappaCoeffs_)
{}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize> >
mousse::polynomialTransport<Thermo, PolySize>::clone() const
{
  return autoPtr<polynomialTransport<Thermo, PolySize> >
  (
    new polynomialTransport<Thermo, PolySize>(*this)
  );
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize> >
mousse::polynomialTransport<Thermo, PolySize>::New(Istream& is)
{
  return autoPtr<polynomialTransport<Thermo, PolySize> >
  (
    new polynomialTransport<Thermo, PolySize>(is)
  );
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize> >
mousse::polynomialTransport<Thermo, PolySize>::New(const dictionary& dict)
{
  return autoPtr<polynomialTransport<Thermo, PolySize> >
  (
    new polynomialTransport<Thermo, PolySize>(dict)
  );
}
// Member Functions 
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::mu
(
  const scalar /*p*/,
  const scalar T
) const
{
  return muCoeffs_.value(T)/this->W();
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  return kappaCoeffs_.value(T)/this->W();
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::alphah
(
  const scalar p, const scalar T
) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>&
mousse::polynomialTransport<Thermo, PolySize>::operator=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  Thermo::operator=(pt);
  muCoeffs_ = pt.muCoeffs_;
  kappaCoeffs_ = pt.kappaCoeffs_;
  return *this;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator+=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  muCoeffs_ = molr1*muCoeffs_ + molr2*pt.muCoeffs_;
  kappaCoeffs_ = molr1*kappaCoeffs_ + molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator-=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  muCoeffs_ = molr1*muCoeffs_ - molr2*pt.muCoeffs_;
  kappaCoeffs_ = molr1*kappaCoeffs_ - molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator+
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return polynomialTransport<Thermo, PolySize>
  (
    t,
    molr1*pt1.muCoeffs_ + molr2*pt2.muCoeffs_,
    molr1*pt1.kappaCoeffs_ + molr2*pt2.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator-
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return polynomialTransport<Thermo, PolySize>
  (
    t,
    molr1*pt1.muCoeffs_ - molr2*pt2.muCoeffs_,
    molr1*pt1.kappaCoeffs_ - molr2*pt2.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator*
(
  const scalar s,
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  return polynomialTransport<Thermo, PolySize>
  (
    s*static_cast<const Thermo&>(pt),
    pt.muCoeffs_,
    pt.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator==
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  return pt2 - pt1;
}
