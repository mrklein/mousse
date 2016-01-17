// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoConst
// Description
//   RhoConst (rho = const) of state.
// SourceFiles
//   rho_const.cpp
#ifndef rho_const_hpp_
#define rho_const_hpp_
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class rhoConst;
template<class Specie>
inline rhoConst<Specie> operator+
(
  const rhoConst<Specie>&,
  const rhoConst<Specie>&
);
template<class Specie>
inline rhoConst<Specie> operator-
(
  const rhoConst<Specie>&,
  const rhoConst<Specie>&
);
template<class Specie>
inline rhoConst<Specie> operator*
(
  const scalar,
  const rhoConst<Specie>&
);
template<class Specie>
inline rhoConst<Specie> operator==
(
  const rhoConst<Specie>&,
  const rhoConst<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const rhoConst<Specie>&
);
template<class Specie>
class rhoConst
:
  public Specie
{
  // Private data
    //- Density
    scalar rho_;
public:
  // Constructors
    //- Construct from components
    inline rhoConst(const Specie& sp, const scalar rho);
    //- Construct from Istream
    rhoConst(Istream&);
    //- Construct from dictionary
    rhoConst(const dictionary& dict);
    //- Construct as named copy
    inline rhoConst(const word& name, const rhoConst&);
    //- Construct and return a clone
    inline autoPtr<rhoConst> clone() const;
    // Selector from Istream
    inline static autoPtr<rhoConst> New(Istream& is);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "rhoConst<" + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = true;
      //- Is the equation of state is isochoric i.e. rho = const
      static const bool isochoric = true;
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
    inline void operator+=(const rhoConst&);
    inline void operator-=(const rhoConst&);
    inline void operator*=(const scalar);
  // Friend operators
    friend rhoConst operator+ <Specie>
    (
      const rhoConst&,
      const rhoConst&
    );
    friend rhoConst operator- <Specie>
    (
      const rhoConst&,
      const rhoConst&
    );
    friend rhoConst operator* <Specie>
    (
      const scalar s,
      const rhoConst&
    );
    friend rhoConst operator== <Specie>
    (
      const rhoConst&,
      const rhoConst&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const rhoConst&
    );
};
}  // namespace mousse

// Private Member Functions 
template<class Specie>
inline mousse::rhoConst<Specie>::rhoConst
(
  const Specie& sp,
  const scalar rho
)
:
  Specie{sp},
  rho_{rho}
{}
// Constructors 
template<class Specie>
inline mousse::rhoConst<Specie>::rhoConst
(
  const word& name,
  const rhoConst<Specie>& ico
)
:
  Specie{name, ico},
  rho_{ico.rho_}
{}
template<class Specie>
inline mousse::autoPtr<mousse::rhoConst<Specie> >
mousse::rhoConst<Specie>::clone() const
{
  return {new rhoConst<Specie>{*this}};
}
template<class Specie>
inline mousse::autoPtr<mousse::rhoConst<Specie> >
mousse::rhoConst<Specie>::New(Istream& is)
{
  return {new rhoConst<Specie>{is}};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::rho(scalar /*p*/, scalar /*T*/) const
{
  return rho_;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::s(scalar /*p*/, scalar /*T*/) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::psi(scalar /*p*/, scalar /*T*/) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::Z(scalar /*p*/, scalar /*T*/) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::rhoConst<Specie>::cpMcv(scalar /*p*/, scalar /*T*/) const
{
  return 0;
}
// Member Operators 
template<class Specie>
inline void mousse::rhoConst<Specie>::operator+=(const rhoConst<Specie>& ico)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(ico);
  molr1 /= this->nMoles();
  scalar molr2 = ico.nMoles()/this->nMoles();
  rho_ = molr1*rho_ + molr2*ico.rho_;
}
template<class Specie>
inline void mousse::rhoConst<Specie>::operator-=(const rhoConst<Specie>& ico)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(ico);
  molr1 /= this->nMoles();
  scalar molr2 = ico.nMoles()/this->nMoles();
  rho_ = molr1*rho_ - molr2*ico.rho_;
}
template<class Specie>
inline void mousse::rhoConst<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator+
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  scalar nMoles = ico1.nMoles() + ico2.nMoles();
  scalar molr1 = ico1.nMoles()/nMoles;
  scalar molr2 = ico2.nMoles()/nMoles;
  return {static_cast<const Specie&>(ico1) + static_cast<const Specie&>(ico2),
          molr1*ico1.rho_ + molr2*ico2.rho_};
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator-
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  scalar nMoles = ico1.nMoles() + ico2.nMoles();
  scalar molr1 = ico1.nMoles()/nMoles;
  scalar molr2 = ico2.nMoles()/nMoles;
  return {static_cast<const Specie&>(ico1) - static_cast<const Specie&>(ico2),
          molr1*ico1.rho_ - molr2*ico2.rho_};
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator*
(
  const scalar s,
  const rhoConst<Specie>& ico
)
{
  return {s*static_cast<const Specie&>(ico), ico.rho_};
}
template<class Specie>
inline mousse::rhoConst<Specie> mousse::operator==
(
  const rhoConst<Specie>& ico1,
  const rhoConst<Specie>& ico2
)
{
  return ico2 - ico1;
}
#ifdef NoRepository
#   include "rho_const.cpp"
#endif
#endif
