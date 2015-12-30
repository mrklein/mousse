// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hConstThermo
// Description
//   Constant properties thermodynamics package
//   templated into the EquationOfState.
// SourceFiles
//   h_const_thermo_i.hpp
//   h_const_thermo.cpp
#ifndef h_const_thermo_hpp_
#define h_const_thermo_hpp_
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState> class hConstThermo;
template<class EquationOfState>
inline hConstThermo<EquationOfState> operator+
(
  const hConstThermo<EquationOfState>&,
  const hConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hConstThermo<EquationOfState> operator-
(
  const hConstThermo<EquationOfState>&,
  const hConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hConstThermo<EquationOfState> operator*
(
  const scalar,
  const hConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hConstThermo<EquationOfState> operator==
(
  const hConstThermo<EquationOfState>&,
  const hConstThermo<EquationOfState>&
);
template<class EquationOfState>
Ostream& operator<<
(
  Ostream&,
  const hConstThermo<EquationOfState>&
);
template<class EquationOfState>
class hConstThermo
:
  public EquationOfState
{
  // Private data
    scalar Cp_;
    scalar Hf_;
  // Private Member Functions
    //- Construct from components
    inline hConstThermo
    (
      const EquationOfState& st,
      const scalar cp,
      const scalar hf
    );
public:
  // Constructors
    //- Construct from Istream
    hConstThermo(Istream&);
    //- Construct from dictionary
    hConstThermo(const dictionary& dict);
    //- Construct as named copy
    inline hConstThermo(const word&, const hConstThermo&);
    //- Construct and return a clone
    inline autoPtr<hConstThermo> clone() const;
    //- Selector from Istream
    inline static autoPtr<hConstThermo> New(Istream& is);
    //- Selector from dictionary
    inline static autoPtr<hConstThermo> New(const dictionary& dict);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "hConst<" + EquationOfState::typeName() + '>';
    }
    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const;
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
    inline void operator+=(const hConstThermo&);
    inline void operator-=(const hConstThermo&);
  // Friend operators
    friend hConstThermo operator+ <EquationOfState>
    (
      const hConstThermo&,
      const hConstThermo&
    );
    friend hConstThermo operator- <EquationOfState>
    (
      const hConstThermo&,
      const hConstThermo&
    );
    friend hConstThermo operator* <EquationOfState>
    (
      const scalar,
      const hConstThermo&
    );
    friend hConstThermo operator== <EquationOfState>
    (
      const hConstThermo&,
      const hConstThermo&
    );
  // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
      Ostream&,
      const hConstThermo&
    );
};
}  // namespace mousse
#include "h_const_thermo_i.hpp"
#ifdef NoRepository
#   include "h_const_thermo.cpp"
#endif
#endif
