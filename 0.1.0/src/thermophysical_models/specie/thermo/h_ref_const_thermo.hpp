// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hRefConstThermo
// Description
//   Constant properties thermodynamics package
//   templated into the EquationOfState.
// SourceFiles
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

// Private Member Functions 
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState>::hRefConstThermo
(
  const EquationOfState& st,
  const scalar cp,
  const scalar hf,
  const scalar tref,
  const scalar href
)
:
  EquationOfState{st},
  Cp_{cp},
  Hf_{hf},
  Tref_{tref},
  Href_{href}
{}
// Constructors 
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState>::hRefConstThermo
(
  const word& name,
  const hRefConstThermo& ct
)
:
  EquationOfState{name, ct},
  Cp_{ct.Cp_},
  Hf_{ct.Hf_},
  Tref_{ct.Tref_},
  Href_{ct.Href_}
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::clone() const
{
  return {new hRefConstThermo<EquationOfState>{*this}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::New(Istream& is)
{
  return {new hRefConstThermo<EquationOfState>{is}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hRefConstThermo<EquationOfState> >
mousse::hRefConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return {new hRefConstThermo<EquationOfState>{dict}};
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::cp
(
  const scalar p,
  const scalar T
) const
{
  return Cp_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::ha
(
  const scalar p, const scalar T
) const
{
  return Cp_*(T-Tref_) + Href_ + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::hs
(
  const scalar p, const scalar T
) const
{
  return Cp_*(T-Tref_) + Href_ ;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hRefConstThermo<EquationOfState>::s
(
  const scalar p, const scalar T
) const
{
  return Cp_*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::hRefConstThermo<EquationOfState>::operator+=
(
  const hRefConstThermo<EquationOfState>& ct
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
inline void mousse::hRefConstThermo<EquationOfState>::operator-=
(
  const hRefConstThermo<EquationOfState>& ct
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
inline mousse::hRefConstThermo<EquationOfState> mousse::operator+
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
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
            + ct2.nMoles()/eofs.nMoles()*ct2.Hf_,
          ct1.nMoles()/eofs.nMoles()*ct1.Tref_
            + ct2.nMoles()/eofs.nMoles()*ct2.Tref_,
          ct1.nMoles()/eofs.nMoles()*ct1.Href_
            + ct2.nMoles()/eofs.nMoles()*ct2.Href_};
}
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator-
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
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
inline mousse::hRefConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const hRefConstThermo<EquationOfState>& ct
)
{
  return {s*static_cast<const EquationOfState&>(ct),
          ct.Cp_,
          ct.Hf_,
          ct.Tref_,
          ct.Href_};
}
template<class EquationOfState>
inline mousse::hRefConstThermo<EquationOfState> mousse::operator==
(
  const hRefConstThermo<EquationOfState>& ct1,
  const hRefConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
#ifdef NoRepository
#   include "h_ref_const_thermo.cpp"
#endif
#endif
