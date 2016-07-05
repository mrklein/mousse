#ifndef THERMOPHYSICAL_MODELS_SPECIE_THERMO_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_THERMO_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermo
// Description
//   Basic thermodynamics type based on the use of fitting functions for
//   cp, h, s obtained from the template argument type thermo.  All other
//   properties are derived from these primitive functions.

#include "thermodynamic_constants.hpp"


using namespace mousse::constant::thermodynamic;

namespace mousse {
namespace species {

// Forward declaration of friend functions and operators
template<class Thermo, template<class> class Type> class thermo;

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator+
(
  const thermo<Thermo, Type>&,
  const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator-
(
  const thermo<Thermo, Type>&,
  const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator*
(
  const scalar,
  const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator==
(
  const thermo<Thermo, Type>&,
  const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
Ostream& operator<<
(
  Ostream&,
  const thermo<Thermo, Type>&
);


template<class Thermo, template<class> class Type>
class thermo
:
  public Thermo,
  public Type<thermo<Thermo, Type>>
{
  // Private data
    //- Convergence tolerance of energy -> temperature inversion functions
    static const scalar tol_;
    //- Max number of iterations in energy->temperature inversion functions
    static const int maxIter_;
  // Private Member Functions
    //- Return the temperature corresponding to the value of the
    //  thermodynamic property f, given the function f = F(p, T)
    //  and dF(p, T)/dT
    inline scalar T
    (
      scalar f,
      scalar p,
      scalar T0,
      scalar (thermo::*F)(const scalar, const scalar) const,
      scalar (thermo::*dFdT)(const scalar, const scalar) const,
      scalar (thermo::*limit)(const scalar) const
    ) const;
public:
  //- The thermodynamics of the individual species'
  typedef thermo<Thermo, Type> thermoType;
  // Constructors
    //- Construct from components
    inline thermo(const Thermo& sp);
    //- Construct from Istream
    thermo(Istream&);
    //- Construct from dictionary
    thermo(const dictionary& dict);
    //- Construct as named copy
    inline thermo(const word& name, const thermo&);
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return
         Thermo::typeName() + ',' + Type<thermo<Thermo, Type>>::typeName();
    }
    // Fundamental properties
    // (These functions must be provided in derived types)
      // Heat capacity at constant pressure [J/(kmol K)]
      // scalar cp(const scalar) const;
      // Absolute Enthalpy [J/kmol]
      // scalar ha(const scalar) const;
      // Sensible enthalpy [J/kmol]
      // scalar hs(const scalar) const;
      // Chemical enthalpy [J/kmol]
      // scalar hc(const scalar) const;
      // Entropy [J/(kmol K)]
      // scalar s(const scalar) const;
    // Calculate and return derived properties
    // (These functions need not provided in derived types)
      // Mole specific properties
        //- Name of Enthalpy/Internal energy
        static inline word heName();
        //- Enthalpy/Internal energy [J/kmol]
        inline scalar he(const scalar p, const scalar T) const;
        //- Heat capacity at constant volume [J/(kmol K)]
        inline scalar cv(const scalar p, const scalar T) const;
        //- Heat capacity at constant pressure/volume [J/(kmol K)]
        inline scalar cpv(const scalar p, const scalar T) const;
        //- Gamma = cp/cv []
        inline scalar gamma(const scalar p, const scalar T) const;
        //- Ratio of heat capacity at constant pressure to that at
        //  constant pressure/volume []
        inline scalar cpBycpv(const scalar p, const scalar T) const;
        //- Sensible internal energy [J/kmol]
        inline scalar es(const scalar p, const scalar T) const;
        //- Absolute internal energy [J/kmol]
        inline scalar ea(const scalar p, const scalar T) const;
        //- Gibbs free energy [J/kmol]
        inline scalar g(const scalar p, const scalar T) const;
        //- Helmholtz free energy [J/kmol]
        inline scalar a(const scalar p, const scalar T) const;
      // Mass specific properties
        //- Heat capacity at constant pressure [J/(kg K)]
        inline scalar Cp(const scalar p, const scalar T) const;
        //- Heat capacity at constant volume [J/(kg K)]
        inline scalar Cv(const scalar p, const scalar T) const;
        //- Heat capacity at constant pressure/volume [J/(kg K)]
        inline scalar Cpv(const scalar p, const scalar T) const;
        //- Enthalpy/Internal energy [J/kg]
        inline scalar HE(const scalar p, const scalar T) const;
        //- Sensible enthalpy [J/kg]
        inline scalar Hs(const scalar p, const scalar T) const;
        //- Chemical enthalpy [J/kg]
        inline scalar Hc() const;
        //- Absolute Enthalpy [J/kg]
        inline scalar Ha(const scalar p, const scalar T) const;
        //- Entropy [J/(kg K)]
        inline scalar S(const scalar p, const scalar T) const;
        //- Internal energy [J/kg]
        inline scalar E(const scalar p, const scalar T) const;
        //- Sensible internal energy [J/kg]
        inline scalar Es(const scalar p, const scalar T) const;
        //- Absolute internal energy [J/kg]
        inline scalar Ea(const scalar p, const scalar T) const;
        //- Gibbs free energy [J/kg]
        inline scalar G(const scalar p, const scalar T) const;
        //- Helmholtz free energy [J/kg]
        inline scalar A(const scalar p, const scalar T) const;
    // Equilibrium reaction thermodynamics
      //- Equilibrium constant [] i.t.o fugacities
      //  = PIi(fi/Pstd)^nui
      inline scalar K(const scalar p, const scalar T) const;
      //- Equilibrium constant [] i.t.o. partial pressures
      //  = PIi(pi/Pstd)^nui
      //  For low pressures (where the gas mixture is near perfect) Kp = K
      inline scalar Kp(const scalar p, const scalar T) const;
      //- Equilibrium constant i.t.o. molar concentration
      //  = PIi(ci/cstd)^nui
      //  For low pressures (where the gas mixture is near perfect)
      //  Kc = Kp(pstd/(RR*T))^nu
      inline scalar Kc(const scalar p, const scalar T) const;
      //- Equilibrium constant [] i.t.o. mole-fractions
      //  For low pressures (where the gas mixture is near perfect)
      //  Kx = Kp(pstd/p)^nui
      inline scalar Kx
      (
        const scalar p,
        const scalar T
      ) const;
      //- Equilibrium constant [] i.t.o. number of moles
      //  For low pressures (where the gas mixture is near perfect)
      //  Kn = Kp(n*pstd/p)^nui where n = number of moles in mixture
      inline scalar Kn
      (
        const scalar p,
        const scalar T,
        const scalar n
      ) const;
    // Energy->temperature  inversion functions
      //- Temperature from enthalpy or internal energy
      //  given an initial temperature T0
      inline scalar THE
      (
        const scalar H,
        const scalar p,
        const scalar T0
      ) const;
      //- Temperature from sensible enthalpy given an initial T0
      inline scalar THs
      (
        const scalar Hs,
        const scalar p,
        const scalar T0
      ) const;
      //- Temperature from absolute enthalpy
      //  given an initial temperature T0
      inline scalar THa
      (
        const scalar H,
        const scalar p,
        const scalar T0
      ) const;
      //- Temperature from sensible internal energy
      //  given an initial temperature T0
      inline scalar TEs
      (
        const scalar E,
        const scalar p,
        const scalar T0
      ) const;
      //- Temperature from absolute internal energy
      //  given an initial temperature T0
      inline scalar TEa
      (
        const scalar E,
        const scalar p,
        const scalar T0
      ) const;
    // I-O
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline void operator+=(const thermo&);
    inline void operator-=(const thermo&);
    inline void operator*=(const scalar);
  // Friend operators
    friend thermo operator+ <Thermo, Type>
    (
      const thermo&,
      const thermo&
    );
    friend thermo operator- <Thermo, Type>
    (
      const thermo&,
      const thermo&
    );
    friend thermo operator* <Thermo, Type>
    (
      const scalar s,
      const thermo&
    );
    friend thermo operator== <Thermo, Type>
    (
      const thermo&,
      const thermo&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo, Type>
    (
      Ostream&,
      const thermo&
    );
};

}  // namespace species
}  // namespace mousse


// Private Member Functions 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type>::thermo
(
  const Thermo& sp
)
:
  Thermo{sp}
{}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::T
(
  scalar f,
  scalar p,
  scalar T0,
  scalar (thermo<Thermo, Type>::*F)(const scalar, const scalar) const,
  scalar (thermo<Thermo, Type>::*dFdT)(const scalar, const scalar)
    const,
  scalar (thermo<Thermo, Type>::*limit)(const scalar) const
) const
{
  scalar Test = T0;
  scalar Tnew = T0;
  scalar Ttol = T0*tol_;
  int iter = 0;
  do {
    Test = Tnew;
    Tnew =
      (this->*limit)(Test - ((this->*F)(p, Test) - f)/(this->*dFdT)(p, Test));
    if (iter++ > maxIter_) {
      FATAL_ERROR_IN_FUNCTION
        << "Maximum number of iterations exceeded"
        << abort(FatalError);
    }
  } while (mag(Tnew - Test) > Ttol);
  return Tnew;
}


// Constructors 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type>::thermo
(
  const word& name,
  const thermo& st
)
:
  Thermo{name, st}
{}


// Member Functions 
template<class Thermo, template<class> class Type>
inline mousse::word
mousse::species::thermo<Thermo, Type>::heName()
{
  return Type<thermo<Thermo, Type>>::name();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::he(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type>>::he(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cv(const scalar p, const scalar T) const
{
  return this->cp(p, T) - this->cpMcv(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cpv(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type>>::cpv(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::gamma(const scalar p, const scalar T) const
{
  scalar cp = this->cp(p, T);
  return cp/(cp - this->cpMcv(p, T));
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cpBycpv
(
  const scalar p,
  const scalar T
) const
{
  return Type<thermo<Thermo, Type>>::cpBycpv(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::es(const scalar p, const scalar T) const
{
  return this->hs(p, T) - p*this->W()/this->rho(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::ea(const scalar p, const scalar T) const
{
  return this->ha(p, T) - p*this->W()/this->rho(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::g(const scalar p, const scalar T) const
{
  return this->ha(p, T) - T*this->s(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::a(const scalar p, const scalar T) const
{
  return this->ea(p, T) - T*this->s(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cpv(const scalar p, const scalar T) const
{
  return this->cpv(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cp(const scalar p, const scalar T) const
{
  return this->cp(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cv(const scalar p, const scalar T) const
{
  return this->cv(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::HE(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type>>::HE(*this, p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Hs(const scalar p, const scalar T) const
{
  return this->hs(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Hc() const
{
  return this->hc()/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Ha(const scalar p, const scalar T) const
{
  return this->ha(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::S(const scalar p, const scalar T) const
{
  return this->s(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::E(const scalar p, const scalar T) const
{
  return this->e(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Es(const scalar p, const scalar T) const
{
  return this->es(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Ea(const scalar p, const scalar T) const
{
  return this->ea(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::G(const scalar p, const scalar T) const
{
  return this->g(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::A(const scalar p, const scalar T) const
{
  return this->a(p, T)/this->W();
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::K(const scalar /*p*/, const scalar T) const
{
  scalar arg = -this->nMoles()*this->g(Pstd, T)/(RR*T);
  if (arg < 600.0) {
    return exp(arg);
  } else {
    return VGREAT;
  }
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Kp(const scalar p, const scalar T) const
{
  return K(p, T);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Kc(const scalar p, const scalar T) const
{
  if (equal(this->nMoles(), SMALL)) {
    return Kp(p, T);
  } else {
    return Kp(p, T)*pow(Pstd/(RR*T), this->nMoles());
  }
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::Kx
(
  const scalar p,
  const scalar T
) const
{
  if (equal(this->nMoles(), SMALL)) {
    return Kp(p, T);
  } else {
    return Kp(p, T)*pow(Pstd/p, this->nMoles());
  }
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::Kn
(
  const scalar p,
  const scalar T,
  const scalar n
) const
{
  if (equal(this->nMoles(), SMALL)) {
    return Kp(p, T);
  } else {
    return Kp(p, T)*pow(n*Pstd/p, this->nMoles());
  }
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THE
(
  const scalar he,
  const scalar p,
  const scalar T0
) const
{
  return Type<thermo<Thermo, Type>>::THE(*this, he, p, T0);
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THs
(
  const scalar hs,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    hs,
    p,
    T0,
    &thermo<Thermo, Type>::Hs,
    &thermo<Thermo, Type>::Cp,
    &thermo<Thermo, Type>::limit
  );
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THa
(
  const scalar ha,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    ha,
    p,
    T0,
    &thermo<Thermo, Type>::Ha,
    &thermo<Thermo, Type>::Cp,
    &thermo<Thermo, Type>::limit
  );
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::TEs
(
  const scalar es,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    es,
    p,
    T0,
    &thermo<Thermo, Type>::Es,
    &thermo<Thermo, Type>::Cv,
    &thermo<Thermo, Type>::limit
  );
}


template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::TEa
(
  const scalar ea,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    ea,
    p,
    T0,
    &thermo<Thermo, Type>::Ea,
    &thermo<Thermo, Type>::Cv,
    &thermo<Thermo, Type>::limit
  );
}


// Member Operators 
template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator+=
(
  const thermo<Thermo, Type>& st
)
{
  Thermo::operator+=(st);
}


template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator-=
(
  const thermo<Thermo, Type>& st
)
{
  Thermo::operator-=(st);
}


template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator*=(const scalar s)
{
  Thermo::operator*=(s);
}


// Friend Operators 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator+
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return {static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)};
}


template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator-
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return {static_cast<const Thermo&>(st1) - static_cast<const Thermo&>(st2)};
}


template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator*
(
  const scalar s,
  const thermo<Thermo, Type>& st
)
{
  return {s*static_cast<const Thermo&>(st)};
}


template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator==
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return st2 - st1;
}

#include "thermo.ipp"

#endif
