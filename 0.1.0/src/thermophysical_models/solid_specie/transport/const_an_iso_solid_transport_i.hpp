// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
  const Thermo& t,
  const vector kappa
)
:
  Thermo(t),
  kappa_(kappa)
{}
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
  const word& name,
  const constAnIsoSolidTransport& ct
)
:
  Thermo(name, ct),
  kappa_(ct.kappa_)
{}
template<class Thermo>
inline mousse::autoPtr<mousse::constAnIsoSolidTransport<Thermo> >
mousse::constAnIsoSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<constAnIsoSolidTransport<Thermo> >
  (
    new constAnIsoSolidTransport<Thermo>(dict)
  );
}
// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::constAnIsoSolidTransport<Thermo>::
kappa(const scalar p, const scalar T) const
{
  return mag(kappa_);
}
template<class Thermo>
inline mousse::vector mousse::constAnIsoSolidTransport<Thermo>::
Kappa(const scalar p, const scalar T) const
{
  return kappa_;
}
template<class Thermo>
inline mousse::scalar mousse::constAnIsoSolidTransport<Thermo>::
mu(const scalar p, const scalar T) const
{
  notImplemented
  (
    "mousse::scalar mousse::constAnIsoSolidTransport<Thermo>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}
template<class Thermo>
inline mousse::vector mousse::constAnIsoSolidTransport<Thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa_/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>&
mousse::constAnIsoSolidTransport<Thermo>::operator=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  kappa_ = ct.kappa_;
  return *this;
}
template<class Thermo>
inline void mousse::constAnIsoSolidTransport<Thermo>::operator+=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ + molr2*ct.kappa_;
}
template<class Thermo>
inline void mousse::constAnIsoSolidTransport<Thermo>::operator-=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ - molr2*ct.kappa_;
}
// Friend Operators 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo> mousse::operator*
(
  const scalar s,
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  return constAnIsoSolidTransport<Thermo>
  (
    s*static_cast<const Thermo&>(ct),
    ct.kappa_
  );
}
