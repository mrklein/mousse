#ifndef THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_PERFECT_FLUID_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_PERFECT_FLUID_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::perfectFluid
// Description
//   Perfect gas equation of state.

#include "auto_ptr.hpp"
#include "specie.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class Specie> class perfectFluid;

template<class Specie>
inline perfectFluid<Specie> operator+
(
  const perfectFluid<Specie>&,
  const perfectFluid<Specie>&
);

template<class Specie>
inline perfectFluid<Specie> operator-
(
  const perfectFluid<Specie>&,
  const perfectFluid<Specie>&
);

template<class Specie>
inline perfectFluid<Specie> operator*
(
  const scalar,
  const perfectFluid<Specie>&
);

template<class Specie>
inline perfectFluid<Specie> operator==
(
  const perfectFluid<Specie>&,
  const perfectFluid<Specie>&
);

template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const perfectFluid<Specie>&
);


template<class Specie>
class perfectFluid
:
  public Specie
{
  // Private data
    //- Fluid constant
    scalar R_;
    //- The reference density
    scalar rho0_;
public:
  // Constructors
    //- Construct from components
    inline perfectFluid
    (
      const Specie& sp,
      const scalar R,
      const scalar rho0
    );
    //- Construct from Istream
    perfectFluid(Istream&);
    //- Construct from dictionary
    perfectFluid(const dictionary& dict);
    //- Construct as named copy
    inline perfectFluid(const word& name, const perfectFluid&);
    //- Construct and return a clone
    inline autoPtr<perfectFluid> clone() const;
    // Selector from Istream
    inline static autoPtr<perfectFluid> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<perfectFluid> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "perfectFluid<" + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = false;
      //- Is the equation of state is isochoric i.e. rho = const
      static const bool isochoric = false;
      //- Return fluid constant [J/(kg K)]
      inline scalar R() const;
      //- Return density [kg/m^3]
      inline scalar rho(scalar p, scalar T) const;
      //- Return entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
      //- Return compressibility rho/p [s^2/m^2]
      inline scalar psi(scalar p, scalar T) const;
      //- Return compression factor []
      inline scalar Z(scalar p, scalar T) const;
      //- Return (cp - cv) [J/(kmol K]
      inline scalar cpMcv(scalar p, scalar T) const;
    // IO
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline void operator+=(const perfectFluid&);
    inline void operator-=(const perfectFluid&);
    inline void operator*=(const scalar);
  // Friend operators
    friend perfectFluid operator+ <Specie>
    (
      const perfectFluid&,
      const perfectFluid&
    );
    friend perfectFluid operator- <Specie>
    (
      const perfectFluid&,
      const perfectFluid&
    );
    friend perfectFluid operator* <Specie>
    (
      const scalar s,
      const perfectFluid&
    );
    friend perfectFluid operator== <Specie>
    (
      const perfectFluid&,
      const perfectFluid&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const perfectFluid&
    );
};

}  // namespace mousse


// Private Member Functions 
template<class Specie>
inline mousse::perfectFluid<Specie>::perfectFluid
(
  const Specie& sp,
  const scalar R,
  const scalar rho0
)
:
  Specie{sp},
  R_{R},
  rho0_{rho0}
{}


// Constructors 
template<class Specie>
inline mousse::perfectFluid<Specie>::perfectFluid
(
  const word& name,
  const perfectFluid<Specie>& pf
)
:
  Specie{name, pf},
  R_{pf.R_},
  rho0_{pf.rho0_}
{}


template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::clone() const
{
  return {new perfectFluid<Specie>{*this}};
}


template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::New(Istream& is)
{
  return {new perfectFluid<Specie>{is}};
}


template<class Specie>
inline mousse::autoPtr<mousse::perfectFluid<Specie> >
mousse::perfectFluid<Specie>::New
(
  const dictionary& dict
)
{
  return {new perfectFluid<Specie>{dict}};
}


// Member Functions 
template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::R() const
{
  return R_;
}


template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::rho(scalar p, scalar T) const
{
  return rho0_ + p/(this->R()*T);
}


template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::s(scalar p, scalar /*T*/) const
{
  return -RR*log(p/Pstd);
}


template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::psi(scalar /*p*/, scalar T) const
{
  return 1.0/(this->R()*T);
}


template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::Z(scalar /*p*/, scalar /*T*/) const
{
  return 1;
}


template<class Specie>
inline mousse::scalar mousse::perfectFluid<Specie>::cpMcv(scalar /*p*/, scalar /*T*/) const
{
  return 0;
}


// Member Operators 
template<class Specie>
inline void mousse::perfectFluid<Specie>::operator+=
(
  const perfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  R_ = 1.0/(molr1/R_ + molr2/pf.R_);
  rho0_ = molr1*rho0_ + molr2*pf.rho0_;
}


template<class Specie>
inline void mousse::perfectFluid<Specie>::operator-=
(
  const perfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  R_ = 1.0/(molr1/R_ - molr2/pf.R_);
  rho0_ = molr1*rho0_ - molr2*pf.rho0_;
}


template<class Specie>
inline void mousse::perfectFluid<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}


// Friend Operators 
template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator+
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return
    perfectFluid<Specie>
    {
      static_cast<const Specie&>(pf1) + static_cast<const Specie&>(pf2),
      1.0/(molr1/pf1.R_ + molr2/pf2.R_),
      molr1*pf1.rho0_ + molr2*pf2.rho0_
    };
}


template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator-
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return
    perfectFluid<Specie>
    {
      static_cast<const Specie&>(pf1) - static_cast<const Specie&>(pf2),
      1.0/(molr1/pf1.R_ - molr2/pf2.R_),
      molr1*pf1.rho0_ - molr2*pf2.rho0_
    };
}


template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator*
(
  const scalar s,
  const perfectFluid<Specie>& pf
)
{
  return
    perfectFluid<Specie>
    {
      s*static_cast<const Specie&>(pf),
      pf.R_,
      pf.rho0_
    };
}


template<class Specie>
inline mousse::perfectFluid<Specie> mousse::operator==
(
  const perfectFluid<Specie>& pf1,
  const perfectFluid<Specie>& pf2
)
{
  return pf2 - pf1;
}

#include "perfect_fluid.ipp"

#endif
