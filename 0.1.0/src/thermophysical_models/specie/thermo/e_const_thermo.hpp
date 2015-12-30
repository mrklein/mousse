// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::eConstThermo
// Description
//   Constant properties thermodynamics package templated on an equation of
//   state
// SourceFiles
//   e_const_thermo_i.hpp
//   e_const_thermo.cpp
#ifndef e_const_thermo_hpp_
#define e_const_thermo_hpp_
#include "thermo.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState> class eConstThermo;
template<class EquationOfState>
inline eConstThermo<EquationOfState> operator+
(
  const eConstThermo<EquationOfState>&,
  const eConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline eConstThermo<EquationOfState> operator-
(
  const eConstThermo<EquationOfState>&,
  const eConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline eConstThermo<EquationOfState> operator*
(
  const scalar,
  const eConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline eConstThermo<EquationOfState> operator==
(
  const eConstThermo<EquationOfState>&,
  const eConstThermo<EquationOfState>&
);
template<class EquationOfState>
Ostream& operator<<
(
  Ostream&,
  const eConstThermo<EquationOfState>&
);
template<class EquationOfState>
class eConstThermo
:
  public EquationOfState
{
  // Private data
    //- Heat capacity at constant volume
    //  Note: input in [J/(kg K)], but internally uses [J/(kmol K)]
    scalar Cv_;
    //- Heat of formation
    //  Note: input in [J/kg], but internally uses [J/kmol]
    scalar Hf_;
  // Private Member Functions
    //- Construct from components
    inline eConstThermo
    (
      const EquationOfState& st,
      const scalar cv,
      const scalar hf
    );
public:
  // Constructors
    //- Construct from Istream
    eConstThermo(Istream&);
    //- Construct from dictionary
    eConstThermo(const dictionary& dict);
    //- Construct as named copy
    inline eConstThermo(const word&, const eConstThermo&);
    //- Construct and return a clone
    inline autoPtr<eConstThermo> clone() const;
    // Selector from Istream
    inline static autoPtr<eConstThermo> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<eConstThermo> New(const dictionary& dict);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "eConst<" + EquationOfState::typeName() + '>';
    }
    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const;
    // Fundamental properties
      //- Heat capacity at constant pressure [J/(kmol K)]
      inline scalar cp(const scalar p, const scalar T) const;
      //- Absolute Enthalpy [J/kmol]
      inline scalar ha(const scalar p, const scalar T) const;
      //- Sensible Enthalpy [J/kmol]
      inline scalar hs(const scalar p, const scalar T) const;
      //- Chemical enthalpy [J/kmol]
      inline scalar hc() const;
      //- Entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
    // I-O
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline void operator+=(const eConstThermo&);
    inline void operator-=(const eConstThermo&);
  // Friend operators
    friend eConstThermo operator+ <EquationOfState>
    (
      const eConstThermo&,
      const eConstThermo&
    );
    friend eConstThermo operator- <EquationOfState>
    (
      const eConstThermo&,
      const eConstThermo&
    );
    friend eConstThermo operator* <EquationOfState>
    (
      const scalar,
      const eConstThermo&
    );
    friend eConstThermo operator== <EquationOfState>
    (
      const eConstThermo&,
      const eConstThermo&
    );
  // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
      Ostream&,
      const eConstThermo&
    );
};
}  // namespace mousse
#include "e_const_thermo_i.hpp"
#ifdef NoRepository
#   include "e_const_thermo.cpp"
#endif
#endif
