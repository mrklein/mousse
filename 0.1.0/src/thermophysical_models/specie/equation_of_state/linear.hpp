#ifndef THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_LINEAR_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_LINEAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linear
// Description
//   Linear equation of state with constant compressibility
//   \verbatim
//     rho = rho0 + psi*p
//   \endverbatim
// SourceFiles
//   linear.cpp
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class linear;
template<class Specie>
inline linear<Specie> operator+
(
  const linear<Specie>&,
  const linear<Specie>&
);
template<class Specie>
inline linear<Specie> operator-
(
  const linear<Specie>&,
  const linear<Specie>&
);
template<class Specie>
inline linear<Specie> operator*
(
  const scalar,
  const linear<Specie>&
);
template<class Specie>
inline linear<Specie> operator==
(
  const linear<Specie>&,
  const linear<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const linear<Specie>&
);
template<class Specie>
class linear
:
  public Specie
{
  // Private data
    //- Compressibility
    scalar psi_;
    //- The reference density
    scalar rho0_;
public:
  // Constructors
    //- Construct from components
    inline linear
    (
      const Specie& sp,
      const scalar psi,
      const scalar rho0
    );
    //- Construct from Istream
    linear(Istream&);
    //- Construct from dictionary
    linear(const dictionary& dict);
    //- Construct as named copy
    inline linear(const word& name, const linear&);
    //- Construct and return a clone
    inline autoPtr<linear> clone() const;
    // Selector from Istream
    inline static autoPtr<linear> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<linear> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "linear<" + word(Specie::typeName_()) + '>';
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
    inline void operator+=(const linear&);
    inline void operator-=(const linear&);
    inline void operator*=(const scalar);
  // Friend operators
    friend linear operator+ <Specie>
    (
      const linear&,
      const linear&
    );
    friend linear operator- <Specie>
    (
      const linear&,
      const linear&
    );
    friend linear operator* <Specie>
    (
      const scalar s,
      const linear&
    );
    friend linear operator== <Specie>
    (
      const linear&,
      const linear&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const linear&
    );
};
}  // namespace mousse

// Private Member Functions 
template<class Specie>
inline mousse::linear<Specie>::linear
(
  const Specie& sp,
  const scalar psi,
  const scalar rho0
)
:
  Specie{sp},
  psi_{psi},
  rho0_{rho0}
{}
// Constructors 
template<class Specie>
inline mousse::linear<Specie>::linear
(
  const word& name,
  const linear<Specie>& pf
)
:
  Specie{name, pf},
  psi_{pf.psi_},
  rho0_{pf.rho0_}
{}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::clone() const
{
  return {new linear<Specie>{*this}};
}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::New(Istream& is)
{
  return {new linear<Specie>{is}};
}
template<class Specie>
inline mousse::autoPtr<mousse::linear<Specie> >
mousse::linear<Specie>::New
(
  const dictionary& dict
)
{
  return {new linear<Specie>{dict}};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::rho(scalar p, scalar T) const
{
  return rho0_ + psi_*p;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::s(scalar p, scalar T) const
{
  return -log((rho0_ + psi_*p)/(rho0_ + psi_*Pstd))/(T*psi_);
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::psi(scalar p, scalar T) const
{
  return psi_;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::Z(scalar p, scalar T) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::linear<Specie>::cpMcv(scalar p, scalar T) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::linear<Specie>::operator+=
(
  const linear<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  psi_ = molr1*psi_ + molr2*pf.psi_;
  rho0_ = molr1*rho0_ + molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::linear<Specie>::operator-=
(
  const linear<Specie>& pf
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pf);
  molr1 /= this->nMoles();
  scalar molr2 = pf.nMoles()/this->nMoles();
  psi_ = molr1*psi_ - molr2*pf.psi_;
  rho0_ = molr1*rho0_ - molr2*pf.rho0_;
}
template<class Specie>
inline void mousse::linear<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::linear<Specie> mousse::operator+
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   + static_cast<const Specie&>(pf2),
    molr1*pf1.psi_ + molr2*pf2.psi_,
    molr1*pf1.rho0_ + molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator-
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  scalar nMoles = pf1.nMoles() + pf2.nMoles();
  scalar molr1 = pf1.nMoles()/nMoles;
  scalar molr2 = pf2.nMoles()/nMoles;
  return rhoConst<Specie>
  (
    static_cast<const Specie&>(pf1)
   - static_cast<const Specie&>(pf2),
    molr1*pf1.psi_ - molr2*pf2.psi_,
    molr1*pf1.rho0_ - molr2*pf2.rho0_
  );
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator*
(
  const scalar s,
  const linear<Specie>& pf
)
{
  return {s*static_cast<const Specie&>(pf),
          pf.psi_,
          pf.rho0_};
}
template<class Specie>
inline mousse::linear<Specie> mousse::operator==
(
  const linear<Specie>& pf1,
  const linear<Specie>& pf2
)
{
  return pf2 - pf1;
}
#ifdef NoRepository
#   include "linear.cpp"
#endif
#endif
