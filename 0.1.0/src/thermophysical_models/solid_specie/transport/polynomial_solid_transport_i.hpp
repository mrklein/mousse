// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
// Constructors 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const polynomialSolidTransport& pt
)
:
  Thermo(pt),
  kappaCoeffs_(pt.kappaCoeffs_)
{}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const Thermo& t,
  const Polynomial<PolySize>& kappaCoeffs
)
:
  Thermo(t),
  kappaCoeffs_(kappaCoeffs)
{}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const word& name,
  const polynomialSolidTransport& pt
)
:
  Thermo(name, pt),
  kappaCoeffs_(pt.kappaCoeffs_)
{}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::clone() const
{
  return autoPtr<polynomialSolidTransport<Thermo, PolySize> >
  (
    new polynomialSolidTransport<Thermo, PolySize>(*this)
  );
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::New(Istream& is)
{
  return autoPtr<polynomialSolidTransport<Thermo, PolySize> >
  (
    new polynomialSolidTransport<Thermo, PolySize>(is)
  );
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::New(const dictionary& dict)
{
  return autoPtr<polynomialSolidTransport<Thermo, PolySize> >
  (
    new polynomialSolidTransport<Thermo, PolySize>(dict)
  );
}
// Member Functions 
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::mu
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::polynomialSolidTransport<thermo, PolySize>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  return kappaCoeffs_.value(T);
}
template<class Thermo, int PolySize>
inline mousse::vector mousse::polynomialSolidTransport<Thermo, PolySize>::Kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  const scalar kappa(kappaCoeffs_.value(T));
  return vector(kappa, kappa, kappa);
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::alphah
(
  const scalar p, const scalar T
) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>&
mousse::polynomialSolidTransport<Thermo, PolySize>::operator=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  Thermo::operator=(pt);
  kappaCoeffs_ = pt.kappaCoeffs_;
  return *this;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator+=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  kappaCoeffs_ = molr1*kappaCoeffs_ + molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator-=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  kappaCoeffs_ = molr1*kappaCoeffs_ - molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator+
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return polynomialSolidTransport<Thermo, PolySize>
  (
    t,
    molr1*pt1.kappaCoeffs_ + molr2*pt2.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator-
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return polynomialSolidTransport<Thermo, PolySize>
  (
    t,
    molr1*pt1.kappaCoeffs_ - molr2*pt2.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator*
(
  const scalar s,
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  return polynomialSolidTransport<Thermo, PolySize>
  (
    s*static_cast<const Thermo&>(pt),
    pt.kappaCoeffs_
  );
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator==
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  return pt2 - pt1;
}
