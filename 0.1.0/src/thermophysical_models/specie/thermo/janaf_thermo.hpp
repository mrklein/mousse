// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::janafThermo
// Description
//   JANAF tables based thermodynamics package templated
//   into the equation of state.
// SourceFiles
//   janaf_thermo_i.hpp
//   janaf_thermo.cpp
#ifndef janaf_thermo_hpp_
#define janaf_thermo_hpp_
#include "scalar.hpp"
#include "fixed_list.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState> class janafThermo;
template<class EquationOfState>
inline janafThermo<EquationOfState> operator+
(
  const janafThermo<EquationOfState>&,
  const janafThermo<EquationOfState>&
);
template<class EquationOfState>
inline janafThermo<EquationOfState> operator-
(
  const janafThermo<EquationOfState>&,
  const janafThermo<EquationOfState>&
);
template<class EquationOfState>
inline janafThermo<EquationOfState> operator*
(
  const scalar,
  const janafThermo<EquationOfState>&
);
template<class EquationOfState>
inline janafThermo<EquationOfState> operator==
(
  const janafThermo<EquationOfState>&,
  const janafThermo<EquationOfState>&
);
template<class EquationOfState>
Ostream& operator<<
(
  Ostream&,
  const janafThermo<EquationOfState>&
);
template<class EquationOfState>
class janafThermo
:
  public EquationOfState
{
public:
  // Public data
    static const int nCoeffs_ = 7;
    typedef FixedList<scalar, nCoeffs_> coeffArray;
private:
  // Private data
    // Temperature limits of applicability of functions
    scalar Tlow_, Thigh_, Tcommon_;
    coeffArray highCpCoeffs_;
    coeffArray lowCpCoeffs_;
  // Private Member Functions
    //- Check that input data is valid
    void checkInputData() const;
    //- Return the coefficients corresponding to the given temperature
    inline const coeffArray& coeffs(const scalar T) const;
public:
  // Constructors
    //- Construct from components
    inline janafThermo
    (
      const EquationOfState& st,
      const scalar Tlow,
      const scalar Thigh,
      const scalar Tcommon,
      const coeffArray& highCpCoeffs,
      const coeffArray& lowCpCoeffs
    );
    //- Construct from Istream
    janafThermo(Istream&);
    //- Construct from dictionary
    janafThermo(const dictionary& dict);
    //- Construct as a named copy
    inline janafThermo(const word&, const janafThermo&);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "janaf<" + EquationOfState::typeName() + '>';
    }
    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const;
    // Access
      //- Return const access to the low temperature limit
      inline scalar Tlow() const;
      //- Return const access to the high temperature limit
      inline scalar Thigh() const;
      //- Return const access to the common temperature
      inline scalar Tcommon() const;
      //- Return const access to the high temperature poly coefficients
      inline const coeffArray& highCpCoeffs() const;
      //- Return const access to the low temperature poly coefficients
      inline const coeffArray& lowCpCoeffs() const;
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
    inline void operator+=(const janafThermo&);
    inline void operator-=(const janafThermo&);
  // Friend operators
    friend janafThermo operator+ <EquationOfState>
    (
      const janafThermo&,
      const janafThermo&
    );
    friend janafThermo operator- <EquationOfState>
    (
      const janafThermo&,
      const janafThermo&
    );
    friend janafThermo operator* <EquationOfState>
    (
      const scalar,
      const janafThermo&
    );
    friend janafThermo operator== <EquationOfState>
    (
      const janafThermo&,
      const janafThermo&
    );
  // Ostream Operator
    friend Ostream& operator<< <EquationOfState>
    (
      Ostream&,
      const janafThermo&
    );
};
}  // namespace mousse
#include "janaf_thermo_i.hpp"
#ifdef NoRepository
#   include "janaf_thermo.cpp"
#endif
#endif
