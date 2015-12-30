// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState>::eConstThermo
(
  const EquationOfState& st,
  const scalar cv,
  const scalar hf
)
:
  EquationOfState(st),
  Cv_(cv),
  Hf_(hf)
{}
// Constructors 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState>::eConstThermo
(
  const word& name,
  const eConstThermo<EquationOfState>& ct
)
:
  EquationOfState(name, ct),
  Cv_(ct.Cv_),
  Hf_(ct.Hf_)
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::clone() const
{
  return autoPtr<eConstThermo<EquationOfState> >
  (
    new eConstThermo<EquationOfState>(*this)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::New(Istream& is)
{
  return autoPtr<eConstThermo<EquationOfState> >
  (
    new eConstThermo<EquationOfState>(is)
  );
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return autoPtr<eConstThermo<EquationOfState> >
  (
    new eConstThermo<EquationOfState>(dict)
  );
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::cp
(
  const scalar p,
  const scalar T
) const
{
  return Cv_ + this->cpMcv(p, T);
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::ha
(
  const scalar p,
  const scalar T
) const
{
  return cp(p, T)*T + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::hs
(
  const scalar p,
  const scalar T
) const
{
  return cp(p, T)*T;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::s
(
  const scalar p,
  const scalar T
) const
{
  return cp()*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::eConstThermo<EquationOfState>::operator+=
(
  const eConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cv_ = molr1*Cv_ + molr2*ct.Cv_;
  Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}
template<class EquationOfState>
inline void mousse::eConstThermo<EquationOfState>::operator-=
(
  const eConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cv_ = molr1*Cv_ - molr2*ct.Cv_;
  Hf_ = molr1*Hf_ - molr2*ct.Hf_;
}
// Friend Operators 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator+
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return eConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cv_
   + ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   + ct2.nMoles()/eofs.nMoles()*ct2.Hf_
  );
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator-
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   - static_cast<const EquationOfState&>(ct2)
  );
  return eConstThermo<EquationOfState>
  (
    eofs,
    ct1.nMoles()/eofs.nMoles()*ct1.Cv_
   - ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
    ct1.nMoles()/eofs.nMoles()*ct1.Hf_
   - ct2.nMoles()/eofs.nMoles()*ct2.Hf_
  );
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const eConstThermo<EquationOfState>& ct
)
{
  return eConstThermo<EquationOfState>
  (
    s*static_cast<const EquationOfState&>(ct),
    ct.Cv_,
    ct.Hf_
  );
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator==
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
