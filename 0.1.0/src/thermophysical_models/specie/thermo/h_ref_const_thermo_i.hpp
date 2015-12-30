// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState>::hRefConstThermo
(
  const EquationOfState& st,
  const scalar cp,
  const scalar hf,
  const scalar tref,
  const scalar href
)
:
  EquationOfState(st),
  Cp_(cp),
  Hf_(hf),
  Tref_(tref),
  Href_(href)
{}
// Constructors 
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState>::hRefConstThermo
(
  const word& name,
  const hRefConstThermo& ct
)
:
  EquationOfState(name, ct),
  Cp_(ct.Cp_),
  Hf_(ct.Hf_),
  Tref_(ct.Tref_),
  Href_(ct.Href_)
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::clone() const
{
  return autoPtr<hRefConstThermo<EquationOfState> >
  (
    new hRefConstThermo<EquationOfState>(*this)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::New(Istream& is)
{
  return autoPtr<hRefConstThermo<EquationOfState> >
  (
    new hRefConstThermo<EquationOfState>(is)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return autoPtr<hRefConstThermo<EquationOfState> >
  (
    new hRefConstThermo<EquationOfState>(dict)
  );
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::cp
(
  const scalar p,
  const scalar T
) const
{
  return Cp_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::ha
(
  const scalar p, const scalar T
) const
{
  return Cp_*(T-Tref_) + Href_ + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::hs
(
  const scalar p, const scalar T
) const
{
  return Cp_*(T-Tref_) + Href_ ;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::s
(
  const scalar p, const scalar T
) const
{
  return Cp_*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::hRefConstThermo<EquationOfState>::operator+=
(
  const hRefConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cp_ = molr1*Cp_ + molr2*ct.Cp_;
  Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}
template<class EquationOfState>
inline void mousse::hRefConstThermo<EquationOfState>::operator-=
(
  const hRefConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cp_ = molr1*Cp_ - molr2*ct.Cp_;
  Hf_ = molr1*Hf_ - molr2*ct.Hf_;
}
// Friend Operators 
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator+
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return hRefConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cp_
   + ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   + ct2.nMoles()/eofs.nMoles()*ct2.Hf_,
    ct1.nMoles()/eofs.nMoles()*ct1.Tref_
   + ct2.nMoles()/eofs.nMoles()*ct2.Tref_,
    ct1.nMoles()/eofs.nMoles()*ct1.Href_
   + ct2.nMoles()/eofs.nMoles()*ct2.Href_
  );
}
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator-
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   - static_cast<const EquationOfState&>(ct2)
  );
  return hRefConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cp_
   - ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   - ct2.nMoles()/eofs.nMoles()*ct2.Hf_
  );
}
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const hRefConstThermo<EquationOfState>& ct
)
{
  return hRefConstThermo<EquationOfState>
  (
    s*static_cast<const EquationOfState&>(ct),
    ct.Cp_,
    ct.Hf_,
    ct.Tref_,
    ct.Href_
  );
}
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator==
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
