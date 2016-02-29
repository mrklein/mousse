#ifndef THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_POLYNOMIAL_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_POLYNOMIAL_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hPolynomialThermo
// Description
//   Thermodynamics package templated on the equation of state, using polynomial
//   functions for cp, h and s
//   Polynomials for h and s derived from cp
// SourceFiles
//   h_polynomial_thermo.cpp
#include "scalar.hpp"
#include "polynomial.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState, int PolySize>
class hPolynomialThermo;
template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator+
(
  const hPolynomialThermo<EquationOfState, PolySize>&,
  const hPolynomialThermo<EquationOfState, PolySize>&
);
template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator-
(
  const hPolynomialThermo<EquationOfState, PolySize>&,
  const hPolynomialThermo<EquationOfState, PolySize>&
);
template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator*
(
  const scalar,
  const hPolynomialThermo<EquationOfState, PolySize>&
);
template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator==
(
  const hPolynomialThermo<EquationOfState, PolySize>&,
  const hPolynomialThermo<EquationOfState, PolySize>&
);
template<class EquationOfState, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const hPolynomialThermo<EquationOfState, PolySize>&
);
template<class EquationOfState, int PolySize=8>
class hPolynomialThermo
:
  public EquationOfState
{
  // Private data
    //- Heat of formation
    //  Note: input in [J/kg], but internally uses [J/kmol]
    scalar Hf_;
    //- Standard entropy
    //  Note: input in [J/kg/K], but internally uses [J/kmol/K]
    scalar Sf_;
    //- Specific heat at constant pressure polynomial coeffs [J/(kg.K)]
    Polynomial<PolySize> CpCoeffs_;
    //- Enthalpy polynomial coeffs - derived from cp [J/kg]
    //  NOTE: relative to Tstd
    typename Polynomial<PolySize>::intPolyType hCoeffs_;
    //- Entropy - derived from Cp [J/(kg.K)] - relative to Tstd
    Polynomial<PolySize> sCoeffs_;
  // Private Member Functions
    //- Construct from components
    inline hPolynomialThermo
    (
      const EquationOfState& pt,
      const scalar Hf,
      const scalar Sf,
      const Polynomial<PolySize>& CpCoeffs,
      const typename Polynomial<PolySize>::intPolyType& hCoeffs,
      const Polynomial<PolySize>& sCoeffs
    );
public:
  // Constructors
    //- Construct from Istream
    hPolynomialThermo(Istream& is);
    //- Construct from dictionary
    hPolynomialThermo(const dictionary& dict);
    //- Construct as copy
    inline hPolynomialThermo(const hPolynomialThermo&);
    //- Construct as a named copy
    inline hPolynomialThermo(const word&, const hPolynomialThermo&);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "hPolynomial<" + EquationOfState::typeName() + '>';
    }
    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar) const;
    // Fundamental properties
      //- Heat capacity at constant pressure [J/(kmol K)]
      inline scalar cp(const scalar p, const scalar T) const;
      //- Absolute Enthalpy [J/kmol]
      inline scalar ha(const scalar p, const scalar T) const;
      //- Sensible enthalpy [J/kmol]
      inline scalar hs(const scalar p, const scalar T) const;
      //- Chemical enthalpy [J/kmol]
      inline scalar hc() const;
      //- Entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
    // I-O
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline hPolynomialThermo& operator=(const hPolynomialThermo&);
    inline void operator+=(const hPolynomialThermo&);
    inline void operator-=(const hPolynomialThermo&);
    inline void operator*=(const scalar);
  // Friend operators
    friend hPolynomialThermo operator+ <EquationOfState, PolySize>
    (
      const hPolynomialThermo&,
      const hPolynomialThermo&
    );
    friend hPolynomialThermo operator- <EquationOfState, PolySize>
    (
      const hPolynomialThermo&,
      const hPolynomialThermo&
    );
    friend hPolynomialThermo operator* <EquationOfState, PolySize>
    (
      const scalar,
      const hPolynomialThermo&
    );
    friend hPolynomialThermo operator== <EquationOfState, PolySize>
    (
      const hPolynomialThermo&,
      const hPolynomialThermo&
    );
  // Ostream Operator
    friend Ostream& operator<< <EquationOfState, PolySize>
    (
      Ostream&,
      const hPolynomialThermo&
    );
};
}  // namespace mousse

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
  EquationOfState{pt},
  Hf_{Hf},
  Sf_{Sf},
  CpCoeffs_{CpCoeffs},
  hCoeffs_{hCoeffs},
  sCoeffs_{sCoeffs}
{}
// Constructors 
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const hPolynomialThermo& pt
)
:
  EquationOfState{pt},
  Hf_{pt.Hf_},
  Sf_{pt.Sf_},
  CpCoeffs_{pt.CpCoeffs_},
  hCoeffs_{pt.hCoeffs_},
  sCoeffs_{pt.sCoeffs_}
{}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const word& name,
  const hPolynomialThermo& pt
)
:
  EquationOfState{name, pt},
  Hf_{pt.Hf_},
  Sf_{pt.Sf_},
  CpCoeffs_{pt.CpCoeffs_},
  hCoeffs_{pt.hCoeffs_},
  sCoeffs_{pt.sCoeffs_}
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
  return {eofs,
          molr1*pt1.Hf_ + molr2*pt2.Hf_,
          molr1*pt1.Sf_ + molr2*pt2.Sf_,
          molr1*pt1.CpCoeffs_ + molr2*pt2.CpCoeffs_,
          molr1*pt1.hCoeffs_ + molr2*pt2.hCoeffs_,
          molr1*pt1.sCoeffs_ + molr2*pt2.sCoeffs_};
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
  return {eofs,
          molr1*pt1.Hf_ - molr2*pt2.Hf_,
          molr1*pt1.Sf_ - molr2*pt2.Sf_,
          molr1*pt1.CpCoeffs_ - molr2*pt2.CpCoeffs_,
          molr1*pt1.hCoeffs_ - molr2*pt2.hCoeffs_,
          molr1*pt1.sCoeffs_ - molr2*pt2.sCoeffs_};
}
template<class EquationOfState, int PolySize>
inline mousse::hPolynomialThermo<EquationOfState, PolySize> mousse::operator*
(
  const scalar s,
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  return {s*static_cast<const EquationOfState&>(pt),
          pt.Hf_,
          pt.Sf_,
          pt.CpCoeffs_,
          pt.hCoeffs_,
          pt.sCoeffs_};
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
#ifdef NoRepository
#   include "h_polynomial_thermo.cpp"
#endif
#endif
