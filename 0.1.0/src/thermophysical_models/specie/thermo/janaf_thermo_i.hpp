// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "janaf_thermo.hpp"
#include "specie.hpp"
// Private Member Functions 
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState>::janafThermo
(
  const EquationOfState& st,
  const scalar Tlow,
  const scalar Thigh,
  const scalar Tcommon,
  const typename janafThermo<EquationOfState>::coeffArray& highCpCoeffs,
  const typename janafThermo<EquationOfState>::coeffArray& lowCpCoeffs
)
:
  EquationOfState(st),
  Tlow_(Tlow),
  Thigh_(Thigh),
  Tcommon_(Tcommon)
{
  for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
  {
    highCpCoeffs_[coefLabel] = highCpCoeffs[coefLabel];
    lowCpCoeffs_[coefLabel] = lowCpCoeffs[coefLabel];
  }
}
template<class EquationOfState>
inline const typename mousse::janafThermo<EquationOfState>::coeffArray&
mousse::janafThermo<EquationOfState>::coeffs
(
  const scalar T
) const
{
  if (T < Tcommon_)
  {
    return lowCpCoeffs_;
  }
  else
  {
    return highCpCoeffs_;
  }
}
// Constructors 
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState>::janafThermo
(
  const word& name,
  const janafThermo& jt
)
:
  EquationOfState(name, jt),
  Tlow_(jt.Tlow_),
  Thigh_(jt.Thigh_),
  Tcommon_(jt.Tcommon_)
{
  for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
  {
    highCpCoeffs_[coefLabel] = jt.highCpCoeffs_[coefLabel];
    lowCpCoeffs_[coefLabel] = jt.lowCpCoeffs_[coefLabel];
  }
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  if (T < Tlow_ || T > Thigh_)
  {
    WARNING_IN
    (
      "janafThermo<EquationOfState>::limit(const scalar T) const"
    )   << "attempt to use janafThermo<EquationOfState>"
       " out of temperature range "
      << Tlow_ << " -> " << Thigh_ << ";  T = " << T
      << endl;
    return min(max(T, Tlow_), Thigh_);
  }
  else
  {
    return T;
  }
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::Tlow() const
{
  return Tlow_;
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::Thigh() const
{
  return Thigh_;
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::Tcommon() const
{
  return Tcommon_;
}
template<class EquationOfState>
inline const typename mousse::janafThermo<EquationOfState>::coeffArray&
mousse::janafThermo<EquationOfState>::highCpCoeffs() const
{
  return highCpCoeffs_;
}
template<class EquationOfState>
inline const typename mousse::janafThermo<EquationOfState>::coeffArray&
mousse::janafThermo<EquationOfState>::lowCpCoeffs() const
{
  return lowCpCoeffs_;
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::cp
(
  const scalar /*p*/,
  const scalar T
) const
{
  const coeffArray& a = coeffs(T);
  return RR*((((a[4]*T + a[3])*T + a[2])*T + a[1])*T + a[0]);
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::ha
(
  const scalar /*p*/,
  const scalar T
) const
{
  const coeffArray& a = coeffs(T);
  return RR*
  (
    ((((a[4]/5.0*T + a[3]/4.0)*T + a[2]/3.0)*T + a[1]/2.0)*T + a[0])*T
   + a[5]
  );
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::hs
(
  const scalar p,
  const scalar T
) const
{
  return ha(p, T) - hc();
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::hc() const
{
  const coeffArray& a = lowCpCoeffs_;
  return RR*
  (
    (
      (((a[4]/5.0*Tstd + a[3]/4.0)*Tstd + a[2]/3.0)*Tstd + a[1]/2.0)*Tstd
     + a[0]
    )*Tstd + a[5]
  );
}
template<class EquationOfState>
inline mousse::scalar mousse::janafThermo<EquationOfState>::s
(
  const scalar p,
  const scalar T
) const
{
  const coeffArray& a = coeffs(T);
  return
  RR*
  (
    (((a[4]/4.0*T + a[3]/3.0)*T + a[2]/2.0)*T + a[1])*T + a[0]*log(T)
   + a[6]
  )
 + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::janafThermo<EquationOfState>::operator+=
(
  const janafThermo<EquationOfState>& jt
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(jt);
  molr1 /= this->nMoles();
  scalar molr2 = jt.nMoles()/this->nMoles();
  Tlow_ = max(Tlow_, jt.Tlow_);
  Thigh_ = min(Thigh_, jt.Thigh_);
  if (janafThermo<EquationOfState>::debug && notEqual(Tcommon_, jt.Tcommon_))
  {
    FATAL_ERROR_IN
    (
      "janafThermo<EquationOfState>::operator+="
      "(const janafThermo<EquationOfState>& jt) const"
    )   << "Tcommon " << Tcommon_ << " for "
      << (this->name().size() ? this->name() : "others")
      << " != " << jt.Tcommon_ << " for "
      << (jt.name().size() ? jt.name() : "others")
      << exit(FatalError);
  }
  for
  (
    label coefLabel=0;
    coefLabel<janafThermo<EquationOfState>::nCoeffs_;
    coefLabel++
  )
  {
    highCpCoeffs_[coefLabel] =
      molr1*highCpCoeffs_[coefLabel]
     + molr2*jt.highCpCoeffs_[coefLabel];
    lowCpCoeffs_[coefLabel] =
      molr1*lowCpCoeffs_[coefLabel]
     + molr2*jt.lowCpCoeffs_[coefLabel];
  }
}
template<class EquationOfState>
inline void mousse::janafThermo<EquationOfState>::operator-=
(
  const janafThermo<EquationOfState>& jt
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(jt);
  molr1 /= this->nMoles();
  scalar molr2 = jt.nMoles()/this->nMoles();
  Tlow_ = max(Tlow_, jt.Tlow_);
  Thigh_ = min(Thigh_, jt.Thigh_);
  if (janafThermo<EquationOfState>::debug && notEqual(Tcommon_, jt.Tcommon_))
  {
    FATAL_ERROR_IN
    (
      "janafThermo<EquationOfState>::operator-="
      "(const janafThermo<EquationOfState>& jt) const"
    )   << "Tcommon " << Tcommon_ << " for "
      << (this->name().size() ? this->name() : "others")
      << " != " << jt.Tcommon_ << " for "
      << (jt.name().size() ? jt.name() : "others")
      << exit(FatalError);
  }
  for
  (
    label coefLabel=0;
    coefLabel<janafThermo<EquationOfState>::nCoeffs_;
    coefLabel++
  )
  {
    highCpCoeffs_[coefLabel] =
      molr1*highCpCoeffs_[coefLabel]
     - molr2*jt.highCpCoeffs_[coefLabel];
    lowCpCoeffs_[coefLabel] =
      molr1*lowCpCoeffs_[coefLabel]
     - molr2*jt.lowCpCoeffs_[coefLabel];
  }
}
// Friend Operators 
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState> mousse::operator+
(
  const janafThermo<EquationOfState>& jt1,
  const janafThermo<EquationOfState>& jt2
)
{
  EquationOfState eofs = jt1;
  eofs += jt2;
  scalar molr1 = jt1.nMoles()/eofs.nMoles();
  scalar molr2 = jt2.nMoles()/eofs.nMoles();
  typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
  typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;
  for
  (
    label coefLabel=0;
    coefLabel<janafThermo<EquationOfState>::nCoeffs_;
    coefLabel++
  )
  {
    highCpCoeffs[coefLabel] =
      molr1*jt1.highCpCoeffs_[coefLabel]
     + molr2*jt2.highCpCoeffs_[coefLabel];
    lowCpCoeffs[coefLabel] =
      molr1*jt1.lowCpCoeffs_[coefLabel]
     + molr2*jt2.lowCpCoeffs_[coefLabel];
  }
  if
  (
    janafThermo<EquationOfState>::debug
  && notEqual(jt1.Tcommon_, jt2.Tcommon_)
  )
  {
    FATAL_ERROR_IN
    (
      "operator+"
      "(const janafThermo<EquationOfState>& jt1,"
      " const janafThermo<EquationOfState>& jt2)"
    )   << "Tcommon " << jt1.Tcommon_ << " for "
      << (jt1.name().size() ? jt1.name() : "others")
      << " != " << jt2.Tcommon_ << " for "
      << (jt2.name().size() ? jt2.name() : "others")
      << exit(FatalError);
  }
  return janafThermo<EquationOfState>
  (
    eofs,
    max(jt1.Tlow_, jt2.Tlow_),
    min(jt1.Thigh_, jt2.Thigh_),
    jt1.Tcommon_,
    highCpCoeffs,
    lowCpCoeffs
  );
}
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState> mousse::operator-
(
  const janafThermo<EquationOfState>& jt1,
  const janafThermo<EquationOfState>& jt2
)
{
  EquationOfState eofs = jt1;
  eofs -= jt2;
  scalar molr1 = jt1.nMoles()/eofs.nMoles();
  scalar molr2 = jt2.nMoles()/eofs.nMoles();
  typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
  typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;
  for
  (
    label coefLabel=0;
    coefLabel<janafThermo<EquationOfState>::nCoeffs_;
    coefLabel++
  )
  {
    highCpCoeffs[coefLabel] =
      molr1*jt1.highCpCoeffs_[coefLabel]
     - molr2*jt2.highCpCoeffs_[coefLabel];
    lowCpCoeffs[coefLabel] =
      molr1*jt1.lowCpCoeffs_[coefLabel]
     - molr2*jt2.lowCpCoeffs_[coefLabel];
  }
  if
  (
    janafThermo<EquationOfState>::debug
  && notEqual(jt1.Tcommon_, jt2.Tcommon_)
  )
  {
    FATAL_ERROR_IN
    (
      "operator-"
      "(const janafThermo<EquationOfState>& jt1,"
      " const janafThermo<EquationOfState>& jt2)"
    )   << "Tcommon " << jt1.Tcommon_ << " for "
      << (jt1.name().size() ? jt1.name() : "others")
      << " != " << jt2.Tcommon_ << " for "
      << (jt2.name().size() ? jt2.name() : "others")
      << exit(FatalError);
  }
  return janafThermo<EquationOfState>
  (
    eofs,
    max(jt1.Tlow_, jt2.Tlow_),
    min(jt1.Thigh_, jt2.Thigh_),
    jt1.Tcommon_,
    highCpCoeffs,
    lowCpCoeffs
  );
}
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const janafThermo<EquationOfState>& jt
)
{
  return janafThermo<EquationOfState>
  (
    s*static_cast<const EquationOfState&>(jt),
    jt.Tlow_,
    jt.Thigh_,
    jt.Tcommon_,
    jt.highCpCoeffs_,
    jt.lowCpCoeffs_
  );
}
template<class EquationOfState>
inline mousse::janafThermo<EquationOfState> mousse::operator==
(
  const janafThermo<EquationOfState>& jt1,
  const janafThermo<EquationOfState>& jt2
)
{
  return jt2 - jt1;
}
