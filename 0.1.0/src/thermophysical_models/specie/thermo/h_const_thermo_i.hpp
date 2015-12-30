// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState>::hConstThermo
(
  const EquationOfState& st,
  const scalar cp,
  const scalar hf
)
:
  EquationOfState(st),
  Cp_(cp),
  Hf_(hf)
{}
// Constructors 
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState>::hConstThermo
(
  const word& name,
  const hConstThermo& ct
)
:
  EquationOfState(name, ct),
  Cp_(ct.Cp_),
  Hf_(ct.Hf_)
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::clone() const
{
  return autoPtr<hConstThermo<EquationOfState> >
  (
    new hConstThermo<EquationOfState>(*this)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::New(Istream& is)
{
  return autoPtr<hConstThermo<EquationOfState> >
  (
    new hConstThermo<EquationOfState>(is)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return autoPtr<hConstThermo<EquationOfState> >
  (
    new hConstThermo<EquationOfState>(dict)
  );
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::cp
(
  const scalar p,
  const scalar T
) const
{
  return Cp_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::ha
(
  const scalar p, const scalar T
) const
{
  return Cp_*T + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::hs
(
  const scalar p, const scalar T
) const
{
  return Cp_*T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::s
(
  const scalar p, const scalar T
) const
{
  return Cp_*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::hConstThermo<EquationOfState>::operator+=
(
  const hConstThermo<EquationOfState>& ct
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
inline void mousse::hConstThermo<EquationOfState>::operator-=
(
  const hConstThermo<EquationOfState>& ct
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
inline mousse::hConstThermo<EquationOfState> mousse::operator+
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return hConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cp_
   + ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   + ct2.nMoles()/eofs.nMoles()*ct2.Hf_
  );
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator-
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   - static_cast<const EquationOfState&>(ct2)
  );
  return hConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cp_
   - ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   - ct2.nMoles()/eofs.nMoles()*ct2.Hf_
  );
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const hConstThermo<EquationOfState>& ct
)
{
  return hConstThermo<EquationOfState>
  (
    s*static_cast<const EquationOfState&>(ct),
    ct.Cp_,
    ct.Hf_
  );
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator==
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
