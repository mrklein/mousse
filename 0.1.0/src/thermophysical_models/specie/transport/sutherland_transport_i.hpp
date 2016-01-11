// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
// Private Member Functions 
template<class Thermo>
inline void mousse::sutherlandTransport<Thermo>::calcCoeffs
(
  const scalar mu1, const scalar T1,
  const scalar mu2, const scalar T2
)
{
  scalar rootT1 = sqrt(T1);
  scalar mu1rootT2 = mu1*sqrt(T2);
  scalar mu2rootT1 = mu2*rootT1;
  Ts_ = (mu2rootT1 - mu1rootT2)/(mu1rootT2/T1 - mu2rootT1/T2);
  As_ = mu1*(1.0 + Ts_/T1)/rootT1;
}
// Constructors 
template<class Thermo>
inline mousse::sutherlandTransport<Thermo>::sutherlandTransport
(
  const Thermo& t,
  const scalar As,
  const scalar Ts
)
:
  Thermo(t),
  As_(As),
  Ts_(Ts)
{}
template<class Thermo>
inline mousse::sutherlandTransport<Thermo>::sutherlandTransport
(
  const Thermo& t,
  const scalar mu1, const scalar T1,
  const scalar mu2, const scalar T2
)
:
  Thermo(t)
{
  calcCoeffs(mu1, T1, mu2, T2);
}
template<class Thermo>
inline mousse::sutherlandTransport<Thermo>::sutherlandTransport
(
  const word& name,
  const sutherlandTransport& st
)
:
  Thermo(name, st),
  As_(st.As_),
  Ts_(st.Ts_)
{}
template<class Thermo>
inline mousse::autoPtr<mousse::sutherlandTransport<Thermo> >
mousse::sutherlandTransport<Thermo>::clone() const
{
  return autoPtr<sutherlandTransport<Thermo> >
  (
    new sutherlandTransport<Thermo>(*this)
  );
}
template<class Thermo>
inline mousse::autoPtr<mousse::sutherlandTransport<Thermo> >
mousse::sutherlandTransport<Thermo>::New
(
  Istream& is
)
{
  return autoPtr<sutherlandTransport<Thermo> >
  (
    new sutherlandTransport<Thermo>(is)
  );
}
template<class Thermo>
inline mousse::autoPtr<mousse::sutherlandTransport<Thermo> >
mousse::sutherlandTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<sutherlandTransport<Thermo> >
  (
    new sutherlandTransport<Thermo>(dict)
  );
}
// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::sutherlandTransport<Thermo>::mu
(
  const scalar /*p*/,
  const scalar T
) const
{
  return As_*::sqrt(T)/(1.0 + Ts_/T);
}
template<class Thermo>
inline mousse::scalar mousse::sutherlandTransport<Thermo>::kappa
(
  const scalar p, const scalar T
) const
{
  scalar Cv_ = this->Cv(p, T);
  return mu(p, T)*Cv_*(1.32 + 1.77*this->R()/Cv_);
}
template<class Thermo>
inline mousse::scalar mousse::sutherlandTransport<Thermo>::alphah
(
  const scalar p,
  const scalar T
) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo>
inline mousse::sutherlandTransport<Thermo>&
mousse::sutherlandTransport<Thermo>::operator=
(
  const sutherlandTransport<Thermo>& st
)
{
  Thermo::operator=(st);
  As_ = st.As_;
  Ts_ = st.Ts_;
  return *this;
}
template<class Thermo>
inline void mousse::sutherlandTransport<Thermo>::operator+=
(
  const sutherlandTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(st);
  molr1 /= this->nMoles();
  scalar molr2 = st.nMoles()/this->nMoles();
  As_ = molr1*As_ + molr2*st.As_;
  Ts_ = molr1*Ts_ + molr2*st.Ts_;
}
template<class Thermo>
inline void mousse::sutherlandTransport<Thermo>::operator-=
(
  const sutherlandTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(st);
  molr1 /= this->nMoles();
  scalar molr2 = st.nMoles()/this->nMoles();
  As_ = molr1*As_ - molr2*st.As_;
  Ts_ = molr1*Ts_ - molr2*st.Ts_;
}
template<class Thermo>
inline void mousse::sutherlandTransport<Thermo>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo>
inline mousse::sutherlandTransport<Thermo> mousse::operator+
(
  const sutherlandTransport<Thermo>& st1,
  const sutherlandTransport<Thermo>& st2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
  );
  scalar molr1 = st1.nMoles()/t.nMoles();
  scalar molr2 = st2.nMoles()/t.nMoles();
  return sutherlandTransport<Thermo>
  (
    t,
    molr1*st1.As_ + molr2*st2.As_,
    molr1*st1.Ts_ + molr2*st2.Ts_
  );
}
template<class Thermo>
inline mousse::sutherlandTransport<Thermo> mousse::operator-
(
  const sutherlandTransport<Thermo>& st1,
  const sutherlandTransport<Thermo>& st2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(st1) - static_cast<const Thermo&>(st2)
  );
  scalar molr1 = st1.nMoles()/t.nMoles();
  scalar molr2 = st2.nMoles()/t.nMoles();
  return sutherlandTransport<Thermo>
  (
    t,
    molr1*st1.As_ - molr2*st2.As_,
    molr1*st1.Ts_ - molr2*st2.Ts_
  );
}
template<class Thermo>
inline mousse::sutherlandTransport<Thermo> mousse::operator*
(
  const scalar s,
  const sutherlandTransport<Thermo>& st
)
{
  return sutherlandTransport<Thermo>
  (
    s*static_cast<const Thermo&>(st),
    st.As_,
    st.Ts_
  );
}
template<class Thermo>
inline mousse::sutherlandTransport<Thermo> mousse::operator==
(
  const sutherlandTransport<Thermo>& st1,
  const sutherlandTransport<Thermo>& st2
)
{
  return st2 - st1;
}
