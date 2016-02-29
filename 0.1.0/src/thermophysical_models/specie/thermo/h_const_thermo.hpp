#ifndef THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_CONST_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_CONST_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hConstThermo
// Description
//   Constant properties thermodynamics package
//   templated into the EquationOfState.
// SourceFiles
//   h_const_thermo.cpp
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

// Private Member Functions 
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState>::hConstThermo
(
  const EquationOfState& st,
  const scalar cp,
  const scalar hf
)
:
  EquationOfState{st},
  Cp_{cp},
  Hf_{hf}
{}
// Constructors 
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState>::hConstThermo
(
  const word& name,
  const hConstThermo& ct
)
:
  EquationOfState{name, ct},
  Cp_{ct.Cp_},
  Hf_{ct.Hf_}
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::clone() const
{
  return {new hConstThermo<EquationOfState>{*this}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::New(Istream& is)
{
  return {new hConstThermo<EquationOfState>{is}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hConstThermo<EquationOfState> >
mousse::hConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return {new hConstThermo<EquationOfState>{dict}};
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::cp
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  return Cp_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::ha
(
  const scalar /*p*/, const scalar T
) const
{
  return Cp_*T + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::hs
(
  const scalar /*p*/, const scalar T
) const
{
  return Cp_*T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hConstThermo<EquationOfState>::s
(
  const scalar p, const scalar T
) const
{
  return Cp_*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::hConstThermo<EquationOfState>::operator+=
(
  const hConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cp_ = molr1*Cp_ + molr2*ct.Cp_;
  Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}
template<class EquationOfState>
inline void mousse::hConstThermo<EquationOfState>::operator-=
(
  const hConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cp_ = molr1*Cp_ - molr2*ct.Cp_;
  Hf_ = molr1*Hf_ - molr2*ct.Hf_;
}
// Friend Operators 
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator+
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
    + static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.Cp_
            + ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            + ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator-
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   - static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.Cp_
            - ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            - ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const hConstThermo<EquationOfState>& ct
)
{
  return {s*static_cast<const EquationOfState&>(ct),
          ct.Cp_,
          ct.Hf_};
}
template<class EquationOfState>
inline mousse::hConstThermo<EquationOfState> mousse::operator==
(
  const hConstThermo<EquationOfState>& ct1,
  const hConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
#ifdef NoRepository
#   include "h_const_thermo.cpp"
#endif
#endif
