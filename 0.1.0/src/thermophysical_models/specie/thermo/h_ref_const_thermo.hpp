// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hRefConstThermo
// Description
//   Constant properties thermodynamics package
//   templated into the EquationOfState.
// SourceFiles
//   h_ref_const_thermo_i.hpp
//   h_ref_const_thermo.cpp
#ifndef h_ref_const_thermo_hpp_
#define h_ref_const_thermo_hpp_
namespace mousse
{
// Forward declaration of friend functions and operators
template<class EquationOfState> class hRefConstThermo;
template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator+
(
  const hRefConstThermo<EquationOfState>&,
  const hRefConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator-
(
  const hRefConstThermo<EquationOfState>&,
  const hRefConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator*
(
  const scalar,
  const hRefConstThermo<EquationOfState>&
);
template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator==
(
  const hRefConstThermo<EquationOfState>&,
  const hRefConstThermo<EquationOfState>&
);
template<class EquationOfState>
Ostream& operator<<
(
  Ostream&,
  const hRefConstThermo<EquationOfState>&
);
template<class EquationOfState>
class hRefConstThermo
:
  public EquationOfState
{
  // Private data
    scalar Cp_;
    scalar Hf_;
    scalar Tref_;
    scalar Href_;
  // Private Member Functions
    //- Construct from components
    inline hRefConstThermo
    (
      const EquationOfState& st,
      const scalar cp,
      const scalar hf,
      const scalar tref,
      const scalar href
    );
public:
  // Constructors
    //- Construct from Istream
    hRefConstThermo(Istream&);
    //- Construct from dictionary
    hRefConstThermo(const dictionary& dict);
    //- Construct as named copy
    inline hRefConstThermo(const word&, const hRefConstThermo&);
    //- Construct and return a clone
    inline autoPtr<hRefConstThermo> clone() const;
    //- Selector from Istream
    inline static autoPtr<hRefConstThermo> New(Istream& is);
    //- Selector from dictionary
    inline static autoPtr<hRefConstThermo> New(const dictionary& dict);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "hRefConst<" + EquationOfState::typeName() + '>';
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
    inline void operator+=(const hRefConstThermo&);
    inline void operator-=(const hRefConstThermo&);
  // Friend operators
    friend hRefConstThermo operator+ <EquationOfState>
    (
      const hRefConstThermo&,
      const hRefConstThermo&
    );
    friend hRefConstThermo operator- <EquationOfState>
    (
      const hRefConstThermo&,
      const hRefConstThermo&
    );
    friend hRefConstThermo operator* <EquationOfState>
    (
      const scalar,
      const hRefConstThermo&
    );
    friend hRefConstThermo operator== <EquationOfState>
    (
      const hRefConstThermo&,
      const hRefConstThermo&
    );
  // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
      Ostream&,
      const hRefConstThermo&
    );
};
}  // namespace mousse
#include "h_ref_const_thermo_i.hpp"
#ifdef NoRepository
#   include "h_ref_const_thermo.cpp"
#endif
#endif
