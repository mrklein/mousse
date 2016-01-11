// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h_polynomial_thermo.hpp"
// Private Member Functions 
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const EquationOfState& pt,
  const scalar Hf,
  const scalar Sf,
  const Polynomial<PolySize>& CpCoeffs,
  const typename Polynomial<PolySize>::intPolyType& hCoeffs,
  const Polynomial<PolySize>& sCoeffs
)
:
  EquationOfState(pt),
  Hf_(Hf),
  Sf_(Sf),
  CpCoeffs_(CpCoeffs),
  hCoeffs_(hCoeffs),
  sCoeffs_(sCoeffs)
{}
// Constructors 
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const hPolynomialThermo& pt
)
:
  EquationOfState(pt),
  Hf_(pt.Hf_),
  Sf_(pt.Sf_),
  CpCoeffs_(pt.CpCoeffs_),
  hCoeffs_(pt.hCoeffs_),
  sCoeffs_(pt.sCoeffs_)
{}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const word& name,
  const hPolynomialThermo& pt
)
:
  EquationOfState(name, pt),
  Hf_(pt.Hf_),
  Sf_(pt.Sf_),
  CpCoeffs_(pt.CpCoeffs_),
  hCoeffs_(pt.hCoeffs_),
  sCoeffs_(pt.sCoeffs_)
{}
// Member Functions 
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::cp
(
  const scalar /*p*/, const scalar T
) const
{
  return CpCoeffs_.value(T);
}
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::ha
(
  const scalar /*p*/, const scalar T
) const
{
  return hCoeffs_.value(T);
}
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::hs
(
  const scalar p, const scalar T
) const
{
  return ha(p, T) - hc();
}
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::hc()
const
{
  return Hf_;
}
template<class EquationOfState, int PolySize>
inline mousse::scalar mousse::hPolynomialThermo<EquationOfState, PolySize>::s
(
  const scalar p,
  const scalar T
) const
{
  return sCoeffs_.value(T) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>&
mousse::hPolynomialThermo<EquationOfState, PolySize>::operator=
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  EquationOfState::operator=(pt);
  Hf_ = pt.Hf_;
  Sf_ = pt.Sf_;
  CpCoeffs_ = pt.CpCoeffs_;
  hCoeffs_ = pt.hCoeffs_;
  sCoeffs_ = pt.sCoeffs_;
  return *this;
}
template<class EquationOfState, int PolySize>
inline void mousse::hPolynomialThermo<EquationOfState, PolySize>::operator+=
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  Hf_ = molr1*Hf_ + molr2*pt.Hf_;
  Sf_ = molr1*Sf_ + molr2*pt.Sf_;
  CpCoeffs_ = molr1*CpCoeffs_ + molr2*pt.CpCoeffs_;
  hCoeffs_ = molr1*hCoeffs_ + molr2*pt.hCoeffs_;
  sCoeffs_ = molr1*sCoeffs_ + molr2*pt.sCoeffs_;
}
template<class EquationOfState, int PolySize>
inline void mousse::hPolynomialThermo<EquationOfState, PolySize>::operator-=
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  Hf_ = molr1*Hf_ - molr2*pt.Hf_;
  Sf_ = molr1*Sf_ - molr2*pt.Sf_;
  CpCoeffs_ = molr1*CpCoeffs_ - molr2*pt.CpCoeffs_;
  hCoeffs_ = molr1*hCoeffs_ - molr2*pt.hCoeffs_;
  sCoeffs_ = molr1*sCoeffs_ - molr2*pt.sCoeffs_;
}
template<class EquationOfState, int PolySize>
inline void mousse::hPolynomialThermo<EquationOfState, PolySize>::operator*=
(
  const scalar s
)
{
  EquationOfState::operator*=(s);
}
// Friend Operators 
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize> mousse::operator+
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt1,
  const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
  EquationOfState eofs = pt1;
  eofs += pt2;
  scalar molr1 = pt1.nMoles()/eofs.nMoles();
  scalar molr2 = pt2.nMoles()/eofs.nMoles();
  return hPolynomialThermo<EquationOfState, PolySize>
  (
    eofs,
    molr1*pt1.Hf_ + molr2*pt2.Hf_,
    molr1*pt1.Sf_ + molr2*pt2.Sf_,
    molr1*pt1.CpCoeffs_ + molr2*pt2.CpCoeffs_,
    molr1*pt1.hCoeffs_ + molr2*pt2.hCoeffs_,
    molr1*pt1.sCoeffs_ + molr2*pt2.sCoeffs_
  );
}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize> mousse::operator-
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt1,
  const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
  EquationOfState eofs = pt1;
  eofs -= pt2;
  scalar molr1 = pt1.nMoles()/eofs.nMoles();
  scalar molr2 = pt2.nMoles()/eofs.nMoles();
  return hPolynomialThermo<EquationOfState, PolySize>
  (
    eofs,
    molr1*pt1.Hf_ - molr2*pt2.Hf_,
    molr1*pt1.Sf_ - molr2*pt2.Sf_,
    molr1*pt1.CpCoeffs_ - molr2*pt2.CpCoeffs_,
    molr1*pt1.hCoeffs_ - molr2*pt2.hCoeffs_,
    molr1*pt1.sCoeffs_ - molr2*pt2.sCoeffs_
  );
}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize> mousse::operator*
(
  const scalar s,
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  return hPolynomialThermo<EquationOfState, PolySize>
  (
    s*static_cast<const EquationOfState&>(pt),
    pt.Hf_,
    pt.Sf_,
    pt.CpCoeffs_,
    pt.hCoeffs_,
    pt.sCoeffs_
  );
}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize> mousse::operator==
(
  const hPolynomialThermo<EquationOfState, PolySize>& pt1,
  const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
  return pt2 - pt1;
}
