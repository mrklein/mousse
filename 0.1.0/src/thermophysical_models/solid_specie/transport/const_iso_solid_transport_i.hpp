// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo>::constIsoSolidTransport
(
  const thermo& t,
  const scalar kappa
)
:
  thermo(t),
  kappa_(kappa)
{}
template<class thermo>
inline mousse::constIsoSolidTransport<thermo>::constIsoSolidTransport
(
  const word& name,
  const constIsoSolidTransport& ct
)
:
  thermo(name, ct),
  kappa_(ct.kappa_)
{}
template<class Thermo>
inline mousse::autoPtr<mousse::constIsoSolidTransport<Thermo> >
mousse::constIsoSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<constIsoSolidTransport<Thermo> >
  (
    new constIsoSolidTransport<Thermo>(dict)
  );
}
// Member Functions 
template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
kappa(const scalar p, const scalar T) const
{
  return kappa_;
}
template<class thermo>
inline mousse::vector mousse::constIsoSolidTransport<thermo>::
Kappa(const scalar p, const scalar T) const
{
  return vector(kappa_, kappa_, kappa_);
}
template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
mu(const scalar p, const scalar T) const
{
  notImplemented
  (
    "mousse::scalar mousse::constIsoSolidTransport<thermo>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}
template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa_/this->Cpv(p, T);
}
// Member Operators 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo>&
mousse::constIsoSolidTransport<thermo>::operator=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  thermo::operator=(ct);
  kappa_ = ct.kappa_;
  return *this;
}
template<class thermo>
inline void mousse::constIsoSolidTransport<thermo>::operator+=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  thermo::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ + molr2*ct.kappa_;
}
template<class thermo>
inline void mousse::constIsoSolidTransport<thermo>::operator-=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  thermo::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ - molr2*ct.kappa_;
}
// Friend Operators 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo> mousse::operator*
(
  const scalar s,
  const constIsoSolidTransport<thermo>& ct
)
{
  return constIsoSolidTransport<thermo>
  (
    s*static_cast<const thermo&>(ct),
    ct.kappa_
  );
}
