// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::eConstThermo
// Description
//   Constant properties thermodynamics package templated on an equation of
//   state
// SourceFiles
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

// Private Member Functions 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState>::eConstThermo
(
  const EquationOfState& st,
  const scalar cv,
  const scalar hf
)
:
  EquationOfState{st},
  Cv_{cv},
  Hf_{hf}
{}
// Constructors 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState>::eConstThermo
(
  const word& name,
  const eConstThermo<EquationOfState>& ct
)
:
  EquationOfState{name, ct},
  Cv_{ct.Cv_},
  Hf_{ct.Hf_}
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::clone() const
{
  return {new eConstThermo<EquationOfState>{*this}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::New(Istream& is)
{
  return {new eConstThermo<EquationOfState>(is)};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::eConstThermo<EquationOfState> >
mousse::eConstThermo<EquationOfState>::New(const dictionary& dict)
{
  return {new eConstThermo<EquationOfState>(dict)};
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::cp
(
  const scalar p,
  const scalar T
) const
{
  return Cv_ + this->cpMcv(p, T);
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::ha
(
  const scalar p,
  const scalar T
) const
{
  return cp(p, T)*T + Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::hs
(
  const scalar p,
  const scalar T
) const
{
  return cp(p, T)*T;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::eConstThermo<EquationOfState>::s
(
  const scalar p,
  const scalar T
) const
{
  return cp()*log(T/Tstd) + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::eConstThermo<EquationOfState>::operator+=
(
  const eConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cv_ = molr1*Cv_ + molr2*ct.Cv_;
  Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}
template<class EquationOfState>
inline void mousse::eConstThermo<EquationOfState>::operator-=
(
  const eConstThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Cv_ = molr1*Cv_ - molr2*ct.Cv_;
  Hf_ = molr1*Hf_ - molr2*ct.Hf_;
}
// Friend Operators 
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator+
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.Cv_
            + ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            + ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator-
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   - static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.Cv_
            - ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            - ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const eConstThermo<EquationOfState>& ct
)
{
  return {s*static_cast<const EquationOfState&>(ct),
          ct.Cv_,
          ct.Hf_};
}
template<class EquationOfState>
inline mousse::eConstThermo<EquationOfState> mousse::operator==
(
  const eConstThermo<EquationOfState>& ct1,
  const eConstThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
#ifdef NoRepository
#   include "e_const_thermo.cpp"
#endif
#endif
