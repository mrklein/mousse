#ifndef THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_POWER_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_THERMO_H_POWER_THERMO_HPP_

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
//   h_power_thermo.cpp
#include "scalar.hpp"
#include "specie.hpp"
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

// Private Member Functions 
template<class EquationOfState>
inline void mousse::hPowerThermo<EquationOfState>::checkT
(
  const scalar T
) const
{
  if (T < 0)
  {
    FATAL_ERROR_IN
    (
      "hPowerThermo<EquationOfState>::checkT(const scalar T) const"
    )   << "attempt to evaluate hPowerThermo<EquationOfState>"
       " for negative temperature " << T
      << abort(FatalError);
  }
}
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState>::hPowerThermo
(
  const word& name,
  const hPowerThermo& jt
)
:
  EquationOfState{name, jt},
  c0_{jt.c0_},
  n0_{jt.n0_},
  Tref_{jt.Tref_},
  Hf_{jt.Hf_}
{}
// Constructors 
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState>::hPowerThermo
(
  const EquationOfState& st,
  const scalar c0,
  const scalar n0,
  const scalar Tref,
  const scalar Hf
)
:
  EquationOfState{st},
  c0_{c0},
  n0_{n0},
  Tref_{Tref},
  Hf_{Hf}
{}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hPowerThermo<EquationOfState>>
mousse::hPowerThermo<EquationOfState>::clone() const
{
  return {new hPowerThermo<EquationOfState>{*this}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hPowerThermo<EquationOfState>>
mousse::hPowerThermo<EquationOfState>::New(Istream& is)
{
  return {new hPowerThermo<EquationOfState>{is}};
}
template<class EquationOfState>
inline mousse::autoPtr<mousse::hPowerThermo<EquationOfState>>
mousse::hPowerThermo<EquationOfState>::New(const dictionary& dict)
{
  return {new hPowerThermo<EquationOfState>{dict}};
}
// Member Functions 
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::limit
(
  const scalar T
) const
{
  return T;
}
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::cp
(
  const scalar /*p*/, const scalar T
) const
{
  return c0_*pow(T/Tref_, n0_);
}
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::ha
(
  const scalar p, const scalar T
) const
{
  return hs(p, T) + hc();
}
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::hs
(
  const scalar /*p*/, const scalar T
) const
{
  return
    c0_*(pow(T, n0_ + 1) - pow(Tstd, n0_ + 1))/(pow(Tref_, n0_)*(n0_ + 1));
}
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::hc() const
{
  return Hf_;
}
template<class EquationOfState>
inline mousse::scalar mousse::hPowerThermo<EquationOfState>::s
(
  const scalar p, const scalar T
) const
{
  return
    c0_*(pow(T, n0_) - pow(Tstd, n0_))/(pow(Tref_, n0_)*n0_)
   + EquationOfState::s(p, T);
}
// Member Operators 
template<class EquationOfState>
inline void mousse::hPowerThermo<EquationOfState>::operator+=
(
  const hPowerThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Hf_ = molr1*Hf_ + molr2*ct.Hf_;
  c0_ = molr1*c0_ + molr2*ct.c0_;
  n0_ = molr1*n0_ + molr2*ct.n0_;
  Tref_ = molr1*Tref_ + molr2*ct.Tref_;
}
template<class EquationOfState>
inline void mousse::hPowerThermo<EquationOfState>::operator-=
(
  const hPowerThermo<EquationOfState>& ct
)
{
  scalar molr1 = this->nMoles();
  EquationOfState::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  Hf_ = molr1*Hf_ - molr2*ct.Hf_;
  c0_ = (molr1*c0_ - molr2*ct.c0_);
  n0_ = (molr1*n0_ - molr2*ct.n0_);
  Tref_ = (molr1*Tref_ - molr2*ct.Tref_);
}
// Friend Operators 
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState> mousse::operator+
(
  const hPowerThermo<EquationOfState>& ct1,
  const hPowerThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.c0_
            + ct2.nMoles()/eofs.nMoles()*ct2.c0_,
          ct1.nMoles()/eofs.nMoles()*ct1.n0_
            + ct2.nMoles()/eofs.nMoles()*ct2.n0_,
          ct1.nMoles()/eofs.nMoles()*ct1.Tref_
            + ct2.nMoles()/eofs.nMoles()*ct2.Tref_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            + ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState> mousse::operator-
(
  const hPowerThermo<EquationOfState>& ct1,
  const hPowerThermo<EquationOfState>& ct2
)
{
  EquationOfState eofs
  (
    static_cast<const EquationOfState&>(ct1)
   + static_cast<const EquationOfState&>(ct2)
  );
  return {eofs,
          ct1.nMoles()/eofs.nMoles()*ct1.c0_
            - ct2.nMoles()/eofs.nMoles()*ct2.c0_,
          ct1.nMoles()/eofs.nMoles()*ct1.n0_
            - ct2.nMoles()/eofs.nMoles()*ct2.n0_,
          ct1.nMoles()/eofs.nMoles()*ct1.Tref_
            - ct2.nMoles()/eofs.nMoles()*ct2.Tref_,
          ct1.nMoles()/eofs.nMoles()*ct1.Hf_
            - ct2.nMoles()/eofs.nMoles()*ct2.Hf_};
}
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState> mousse::operator*
(
  const scalar s,
  const hPowerThermo<EquationOfState>& ct
)
{
  return {s*static_cast<const EquationOfState&>(ct),
          ct.c0_,
          ct.n0_,
          ct.Tref_,
          ct.Hf_};
}
template<class EquationOfState>
inline mousse::hPowerThermo<EquationOfState> mousse::operator==
(
  const hPowerThermo<EquationOfState>& ct1,
  const hPowerThermo<EquationOfState>& ct2
)
{
  return ct2 - ct1;
}
#ifdef NoRepository
#   include "h_power_thermo.cpp"
#endif
#endif
