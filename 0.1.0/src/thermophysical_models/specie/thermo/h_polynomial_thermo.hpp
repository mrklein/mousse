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
//   h_polynomial_thermo_i.hpp
//   h_polynomial_thermo.cpp
#ifndef h_polynomial_thermo_hpp_
#define h_polynomial_thermo_hpp_
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
#include "h_polynomial_thermo_i.hpp"
#ifdef NoRepository
#   include "h_polynomial_thermo.cpp"
#endif
#endif
