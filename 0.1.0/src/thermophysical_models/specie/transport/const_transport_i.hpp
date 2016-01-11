// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Thermo>
inline mousse::constTransport<Thermo>::constTransport
(
  const Thermo& t,
  const scalar mu,
  const scalar Pr
)
:
  Thermo(t),
  mu_(mu),
  rPr_(1.0/Pr)
{}
template<class Thermo>
inline mousse::constTransport<Thermo>::constTransport
(
  const word& name,
  const constTransport& ct
)
:
  Thermo(name, ct),
  mu_(ct.mu_),
  rPr_(ct.rPr_)
{}
template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo> >
mousse::constTransport<Thermo>::clone() const
{
  return autoPtr<constTransport<Thermo> >
  (
    new constTransport<Thermo>(*this)
  );
}
template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo> >
mousse::constTransport<Thermo>::New
(
  Istream& is
)
{
  return autoPtr<constTransport<Thermo> >
  (
    new constTransport<Thermo>(is)
  );
}
template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo> >
mousse::constTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<constTransport<Thermo> >
  (
    new constTransport<Thermo>(dict)
  );
}
// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::mu
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  return mu_;
}
template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::kappa
(
  const scalar p,
  const scalar T
) const
{
  return this->Cp(p, T)*mu(p, T)*rPr_;
}
template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::alphah
(
  const scalar p,
  const scalar T
) const
{
  return mu(p, T)*rPr_;
}
// Member Operators 
template<class Thermo>
inline mousse::constTransport<Thermo>& mousse::constTransport<Thermo>::operator=
(
  const constTransport<Thermo>& ct
)
{
  Thermo::operator=(ct);
  mu_ = ct.mu_;
  rPr_ = ct.rPr_;
  return *this;
}
template<class Thermo>
inline void mousse::constTransport<Thermo>::operator+=
(
  const constTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(st);
  if (mag(molr1) + mag(st.nMoles()) > SMALL)
  {
    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();
    mu_ = molr1*mu_ + molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ + molr2/st.rPr_);
  }
}
template<class Thermo>
inline void mousse::constTransport<Thermo>::operator-=
(
  const constTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(st);
  if (mag(molr1) + mag(st.nMoles()) > SMALL)
  {
    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();
    mu_ = molr1*mu_ - molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ - molr2/st.rPr_);
  }
}
template<class Thermo>
inline void mousse::constTransport<Thermo>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator+
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(ct1) + static_cast<const Thermo&>(ct2)
  );
  if (mag(ct1.nMoles()) + mag(ct2.nMoles()) < SMALL)
  {
    return constTransport<Thermo>
    (
      t,
      0,
      ct1.rPr_
    );
  }
  else
  {
    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();
    return constTransport<Thermo>
    (
      t,
      molr1*ct1.mu_ + molr2*ct2.mu_,
      1.0/(molr1/ct1.rPr_ + molr2/ct2.rPr_)
    );
  }
}
template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator-
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(ct1) - static_cast<const Thermo&>(ct2)
  );
  if (mag(ct1.nMoles()) + mag(ct2.nMoles()) < SMALL)
  {
    return constTransport<Thermo>
    (
      t,
      0,
      ct1.rPr_
    );
  }
  else
  {
    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();
    return constTransport<Thermo>
    (
      t,
      molr1*ct1.mu_ - molr2*ct2.mu_,
      1.0/(molr1/ct1.rPr_ - molr2/ct2.rPr_)
    );
  }
}
template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator*
(
  const scalar s,
  const constTransport<Thermo>& ct
)
{
  return constTransport<Thermo>
  (
    s*static_cast<const Thermo&>(ct),
    ct.mu_,
    1.0/ct.rPr_
  );
}
template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator==
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  return ct2 - ct1;
}
