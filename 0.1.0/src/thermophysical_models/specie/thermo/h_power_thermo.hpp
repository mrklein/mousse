// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hPowerThermo
// Description
//   Power-function based thermodynamics package templated on EquationOfState.
//   In this thermodynamics package the heat capacity is a simple power of
//   temperature:
//     Cp(T) = c0*(T/Tref)^n0;
//   which is particularly suitable for solids.
// SourceFiles
//   h_power_thermo_i.hpp
//   h_power_thermo.cpp
#ifndef h_power_thermo_hpp_
#define h_power_thermo_hpp_
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState> class hPowerThermo;
template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator+
(
  const hPowerThermo<EquationOfState>&,
  const hPowerThermo<EquationOfState>&
);
template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator-
(
  const hPowerThermo<EquationOfState>&,
  const hPowerThermo<EquationOfState>&
);
template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator*
(
  const scalar,
  const hPowerThermo<EquationOfState>&
);
template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator==
(
  const hPowerThermo<EquationOfState>&,
  const hPowerThermo<EquationOfState>&
);
template<class EquationOfState>
Ostream& operator<<
(
  Ostream&,
  const hPowerThermo<EquationOfState>&
);
template<class EquationOfState>
class hPowerThermo
:
  public EquationOfState
{
  // Private data
    scalar c0_;
    scalar n0_;
    scalar Tref_;
    scalar Hf_;
  // Private Member Functions
    //- Check given temperature is within the range of the fitted coeffs
    inline void checkT(const scalar T) const;
    //- Construct from components
    inline hPowerThermo
    (
      const EquationOfState& st,
      const scalar c0,
      const scalar n0,
      const scalar Tref,
      const scalar Hf
    );
public:
  // Constructors
    //- Construct from Istream
    hPowerThermo(Istream&);
    //- Construct from dictionary
    hPowerThermo(const dictionary&);
    //- Construct as a named copy
    inline hPowerThermo
    (
      const word&,
      const hPowerThermo&
    );
    //- Construct and return a clone
    inline autoPtr<hPowerThermo> clone() const;
    //- Selector from Istream
    inline static autoPtr<hPowerThermo> New(Istream& is);
    //- Selector from dictionary
    inline static autoPtr<hPowerThermo> New(const dictionary& dict);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "hPower<" + EquationOfState::typeName() + '>';
    }
    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const;
    // Fundamental properties
      //- Heat capacity at constant pressure [J/(kg K)]
      inline scalar cp(const scalar p, const scalar T) const;
      //- Absolute enthalpy [J/kmol]
      inline scalar ha(const scalar p, const scalar T) const;
      //- Sensible enthalpy [J/kg]
      inline scalar hs(const scalar p, const scalar T) const;
      //- Chemical enthalpy [J/kg]
      inline scalar hc() const;
      //- Entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
  // Member operators
    inline void operator+=(const hPowerThermo&);
    inline void operator-=(const hPowerThermo&);
  // Friend operators
    friend hPowerThermo operator+ <EquationOfState>
    (
      const hPowerThermo&,
      const hPowerThermo&
    );
    friend hPowerThermo operator- <EquationOfState>
    (
      const hPowerThermo&,
      const hPowerThermo&
    );
    friend hPowerThermo operator* <EquationOfState>
    (
      const scalar,
      const hPowerThermo&
    );
    friend hPowerThermo operator== <EquationOfState>
    (
      const hPowerThermo&,
      const hPowerThermo&
    );
  // Ostream Operator
    friend Ostream& operator<< <EquationOfState>
    (
      Ostream&,
      const hPowerThermo&
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "h_power_thermo_i.hpp"
#   include "h_power_thermo.cpp"
#endif
#endif
