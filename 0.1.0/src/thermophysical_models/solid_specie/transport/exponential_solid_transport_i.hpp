// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
  const Thermo& t,
  const scalar kappa0,
  const scalar n0,
  const scalar Tref
)
:
  Thermo(t),
  kappa0_(kappa0),
  n0_(n0),
  Tref_(Tref)
{}
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
  const word& name,
  const exponentialSolidTransport& ct
)
:
  Thermo(name, ct),
  kappa0_(ct.kappa0_),
  n0_(ct.n0_),
  Tref_(ct.Tref_)
{}
template<class Thermo>
inline mousse::autoPtr<mousse::exponentialSolidTransport<Thermo> >
mousse::exponentialSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<exponentialSolidTransport<Thermo> >
  (
    new exponentialSolidTransport<Thermo>(dict)
  );
}
// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::kappa
(
  const scalar p, const scalar T
) const
{
  return (kappa0_*pow(T/Tref_, n0_));
}
template<class Thermo>
inline mousse::vector mousse::exponentialSolidTransport<Thermo>::Kappa
(
  const scalar p, const scalar T
) const
{
  const scalar kappa(kappa0_*pow(T/Tref_, n0_));
  return vector(kappa, kappa, kappa);
}
template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::
mu(const scalar p, const scalar T) const
{
  notImplemented
  (
    "mousse::scalar mousse::exponentialSolidTransport<Thermo>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}
template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>&
mousse::exponentialSolidTransport<Thermo>::operator=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  kappa0_ = ct.kappa0_;
  n0_ = ct.n0_;
  Tref_ = ct.Tref_;
  return *this;
}
template<class Thermo>
inline void mousse::exponentialSolidTransport<Thermo>::operator+=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa0_ = molr1*kappa0_ + molr2*ct.kappa0_;
  n0_ = (molr1*n0_ + molr2*ct.n0_);
  Tref_ = (molr1*Tref_ + molr2*ct.Tref_);
}
template<class Thermo>
inline void mousse::exponentialSolidTransport<Thermo>::operator-=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa0_ = (molr1*kappa0_ - molr2*ct.kappa0_);
  n0_ = (molr1*n0_ - molr2*ct.n0_);
  Tref_ = (molr1*Tref_ - molr2*ct.Tref_);
}
// Friend Operators 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo> mousse::operator*
(
  const scalar s,
  const exponentialSolidTransport<Thermo>& ct
)
{
  return exponentialSolidTransport<Thermo>
  (
    s*static_cast<const Thermo&>(ct),
    ct.kappa0_,
    ct.n0_,
    ct.Tref_
  );
}
