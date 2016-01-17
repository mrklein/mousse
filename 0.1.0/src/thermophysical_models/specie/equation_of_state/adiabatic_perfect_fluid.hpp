// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adiabaticPerfectFluid
// Description
//   AdiabaticPerfect gas equation of state.
// SourceFiles
//   adiabatic_perfect_fluid.cpp
#ifndef adiabatic_perfect_fluid_hpp_
#define adiabatic_perfect_fluid_hpp_
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class adiabaticPerfectFluid;
template<class Specie>
inline adiabaticPerfectFluid<Specie> operator+
(
  const adiabaticPerfectFluid<Specie>&,
  const adiabaticPerfectFluid<Specie>&
);
template<class Specie>
inline adiabaticPerfectFluid<Specie> operator-
(
  const adiabaticPerfectFluid<Specie>&,
  const adiabaticPerfectFluid<Specie>&
);
template<class Specie>
inline adiabaticPerfectFluid<Specie> operator*
(
  const scalar,
  const adiabaticPerfectFluid<Specie>&
);
template<class Specie>
inline adiabaticPerfectFluid<Specie> operator==
(
  const adiabaticPerfectFluid<Specie>&,
  const adiabaticPerfectFluid<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const adiabaticPerfectFluid<Specie>&
);
template<class Specie>
class adiabaticPerfectFluid
:
  public Specie
{
  // Private data
    //- Reference pressure
    scalar p0_;
    //- Reference density
    scalar rho0_;
    //- The isentropic exponent
    scalar gamma_;
    //- Pressure offset for a stiffened gas
    scalar B_;
public:
  // Constructors
    //- Construct from components
    inline adiabaticPerfectFluid
    (
      const Specie& sp,
      const scalar p0,
      const scalar rho0,
      const scalar gamma,
      const scalar B
    );
    //- Construct from Istream
    adiabaticPerfectFluid(Istream&);
    //- Construct from dictionary
    adiabaticPerfectFluid(const dictionary& dict);
    //- Construct as named copy
    inline adiabaticPerfectFluid
    (
      const word& name,
      const adiabaticPerfectFluid&
    );
    //- Construct and return a clone
    inline autoPtr<adiabaticPerfectFluid> clone() const;
    // Selector from Istream
    inline static autoPtr<adiabaticPerfectFluid> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<adiabaticPerfectFluid> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "adiabaticPerfectFluid<" + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = false;
      //- Is the equation of state is isochoric i.e. rho = const
      static const bool isochoric = false;
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
    inline void operator+=(const adiabaticPerfectFluid&);
    inline void operator-=(const adiabaticPerfectFluid&);
    inline void operator*=(const scalar);
  // Friend operators
    friend adiabaticPerfectFluid operator+ <Specie>
    (
      const adiabaticPerfectFluid&,
      const adiabaticPerfectFluid&
    );
    friend adiabaticPerfectFluid operator- <Specie>
    (
      const adiabaticPerfectFluid&,
      const adiabaticPerfectFluid&
    );
    friend adiabaticPerfectFluid operator* <Specie>
    (
      const scalar s,
      const adiabaticPerfectFluid&
    );
    friend adiabaticPerfectFluid operator== <Specie>
    (
      const adiabaticPerfectFluid&,
      const adiabaticPerfectFluid&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const adiabaticPerfectFluid&
    );
};
}  // namespace mousse

// Private Member Functions 
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid
(
  const Specie& sp,
  const scalar p0,
  const scalar rho0,
  const scalar gamma,
  const scalar B
)
:
  Specie{sp},
  p0_{p0},
  rho0_{rho0},
  gamma_{gamma},
  B_{B}
{}
// Constructors 
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid
(
  const word& name,
  const adiabaticPerfectFluid<Specie>& pf
)
:
  Specie{name, pf},
  p0_{pf.p0_},
  rho0_{pf.rho0_},
  gamma_{pf.gamma_},
  B_{pf.B_}
{}
template<class Specie>
inline mousse::autoPtr<mousse::adiabaticPerfectFluid<Specie> >
mousse::adiabaticPerfectFluid<Specie>::clone() const
{
  return {new adiabaticPerfectFluid<Specie>(*this)};
}
template<class Specie>
inline mousse::autoPtr<mousse::adiabaticPerfectFluid<Specie> >
mousse::adiabaticPerfectFluid<Specie>::New(Istream& is)
{
  return {new adiabaticPerfectFluid<Specie>(is)};
}
template<class Specie>
inline mousse::autoPtr<mousse::adiabaticPerfectFluid<Specie> >
mousse::adiabaticPerfectFluid<Specie>::New
(
  const dictionary& dict
)
{
  return {new adiabaticPerfectFluid<Specie>(dict)};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::adiabaticPerfectFluid<Specie>::rho
(
  scalar p,
  scalar /*T*/
) const
{
  return rho0_*pow((p + B_)/(p0_ + B_), 1.0/gamma_);
}
template<class Specie>
inline mousse::scalar mousse::adiabaticPerfectFluid<Specie>::s
(
  scalar p,
  scalar T
) const
{
  scalar n = 1 - 1.0/gamma_;
  return
   -pow(p0_ + B_, 1.0/gamma_)*(pow((p + B_), n) - pow((Pstd + B_), n))
   /(rho0_*T*n);
}
template<class Specie>
inline mousse::scalar mousse::adiabaticPerfectFluid<Specie>::psi
(
  scalar p,
  scalar /*T*/
) const
{
  return
    (rho0_/(gamma_*(p0_ + B_)))
   *pow((p + B_)/(p0_ + B_), 1.0/gamma_ - 1.0);
}
template<class Specie>
inline mousse::scalar mousse::adiabaticPerfectFluid<Specie>::Z(scalar, scalar) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::adiabaticPerfectFluid<Specie>::cpMcv
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::adiabaticPerfectFluid<Specie>::operator+=
(
  const adiabaticPerfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  p0_ = molr1*p0_ + molr2*pf.p0_;
  rho0_ = molr1*rho0_ + molr2*pf.rho0_;
  gamma_ = molr1*gamma_ + molr2*pf.gamma_;
  B_ = molr1*B_ + molr2*pf.B_;
}
template<class Specie>
inline void mousse::adiabaticPerfectFluid<Specie>::operator-=
(
  const adiabaticPerfectFluid<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  p0_ = molr1*p0_ - molr2*pf.p0_;
  rho0_ = molr1*rho0_ - molr2*pf.rho0_;
  gamma_ = molr1*gamma_ - molr2*pf.gamma_;
  B_ = molr1*B_ - molr2*pf.B_;
}
template<class Specie>
inline void mousse::adiabaticPerfectFluid<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie> mousse::operator+
(
  const adiabaticPerfectFluid<Specie>& pf1,
  const adiabaticPerfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   + static_cast<const Specie&>(pf2),
    molr1*pf1.p0_ + molr2*pf2.p0_,
    molr1*pf1.rho0_ + molr2*pf2.rho0_,
    molr1*pf1.gamma_ + molr2*pf2.gamma_,
    molr1*pf1.B_ + molr2*pf2.B_
  );
}
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie> mousse::operator-
(
  const adiabaticPerfectFluid<Specie>& pf1,
  const adiabaticPerfectFluid<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   - static_cast<const Specie&>(pf2),
    molr1*pf1.p0_ - molr2*pf2.p0_,
    molr1*pf1.rho0_ - molr2*pf2.rho0_,
    molr1*pf1.gamma_ - molr2*pf2.gamma_,
    molr1*pf1.B_ - molr2*pf2.B_
  );
}
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie> mousse::operator*
(
  const scalar s,
  const adiabaticPerfectFluid<Specie>& pf
)
{
  return adiabaticPerfectFluid<Specie>
  (
    s*static_cast<const Specie&>(pf),
    pf.p0_,
    pf.rho0_,
    pf.gamma_,
    pf.B_
  );
}
template<class Specie>
inline mousse::adiabaticPerfectFluid<Specie> mousse::operator==
(
  const adiabaticPerfectFluid<Specie>& pf1,
  const adiabaticPerfectFluid<Specie>& pf2
)
{
  return pf2 - pf1;
}
#ifdef NoRepository
#   include "adiabatic_perfect_fluid.cpp"
#endif
#endif
