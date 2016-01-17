// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressiblePerfectGas
// Description
//   Incompressible gas equation of state using a constant reference pressure in
//   the perfect gas equation of state rather than the local pressure so that the
//   density only varies with temperature and composition.
// SourceFiles
//   incompressible_perfect_gas.cpp
#ifndef incompressible_perfect_gas_hpp_
#define incompressible_perfect_gas_hpp_
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class incompressiblePerfectGas;
template<class Specie>
inline incompressiblePerfectGas<Specie> operator+
(
  const incompressiblePerfectGas<Specie>&,
  const incompressiblePerfectGas<Specie>&
);
template<class Specie>
inline incompressiblePerfectGas<Specie> operator-
(
  const incompressiblePerfectGas<Specie>&,
  const incompressiblePerfectGas<Specie>&
);
template<class Specie>
inline incompressiblePerfectGas<Specie> operator*
(
  const scalar,
  const incompressiblePerfectGas<Specie>&
);
template<class Specie>
inline incompressiblePerfectGas<Specie> operator==
(
  const incompressiblePerfectGas<Specie>&,
  const incompressiblePerfectGas<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const incompressiblePerfectGas<Specie>&
);
template<class Specie>
class incompressiblePerfectGas
:
  public Specie
{
  // Private data
    //- Reference pressure
    scalar pRef_;
public:
  // Constructors
    //- Construct from components
    inline incompressiblePerfectGas(const Specie& sp, const scalar pRef);
    //- Construct from incompressiblePerfectGas
    inline incompressiblePerfectGas(const incompressiblePerfectGas& sp);
    //- Construct from Istream
    incompressiblePerfectGas(Istream&);
    //- Construct from dictionary
    incompressiblePerfectGas(const dictionary& dict);
    //- Construct as named copy
    inline incompressiblePerfectGas
    (
      const word& name,
      const incompressiblePerfectGas&
    );
    //- Construct and return a clone
    inline autoPtr<incompressiblePerfectGas> clone() const;
    // Selector from Istream
    inline static autoPtr<incompressiblePerfectGas> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<incompressiblePerfectGas> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return
        "incompressiblePerfectGas<"
       + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = true;
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
    inline incompressiblePerfectGas& operator=
    (
      const incompressiblePerfectGas&
    );
    inline void operator+=(const incompressiblePerfectGas&);
    inline void operator-=(const incompressiblePerfectGas&);
    inline void operator*=(const scalar);
  // Friend operators
    friend incompressiblePerfectGas operator+ <Specie>
    (
      const incompressiblePerfectGas&,
      const incompressiblePerfectGas&
    );
    friend incompressiblePerfectGas operator- <Specie>
    (
      const incompressiblePerfectGas&,
      const incompressiblePerfectGas&
    );
    friend incompressiblePerfectGas operator* <Specie>
    (
      const scalar s,
      const incompressiblePerfectGas&
    );
    friend incompressiblePerfectGas operator== <Specie>
    (
      const incompressiblePerfectGas&,
      const incompressiblePerfectGas&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const incompressiblePerfectGas&
    );
};
}  // namespace mousse

// Constructors 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const Specie& sp,  const scalar pRef
)
:
  Specie{sp},
  pRef_{pRef}
{}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const incompressiblePerfectGas& ipg
)
:
  Specie{ipg},
  pRef_{ipg.pRef_}
{}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const word& name,
  const incompressiblePerfectGas<Specie>& ipg
)
:
  Specie{name, ipg},
  pRef_{ipg.pRef_}
{}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::clone() const
{
  return {new incompressiblePerfectGas<Specie>(*this)};
}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::New
(
  Istream& is
)
{
  return {new incompressiblePerfectGas<Specie>(is)};
}
template<class Specie>
inline mousse::autoPtr<mousse::incompressiblePerfectGas<Specie> >
mousse::incompressiblePerfectGas<Specie>::New
(
  const dictionary& dict
)
{
  return {new incompressiblePerfectGas<Specie>(dict)};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::rho
(
  scalar /*p*/,
  scalar T
) const
{
  return pRef_/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::s
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::psi
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::Z
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie>
inline mousse::scalar mousse::incompressiblePerfectGas<Specie>::cpMcv
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return RR;
}
// Member Operators 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie>&
mousse::incompressiblePerfectGas<Specie>::operator=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  Specie::operator=(ipg);
  pRef_ = ipg.pRef_;
  return *this;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator+=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(ipg);
  molr1 /= this->nMoles();
  scalar molr2 = ipg.nMoles()/this->nMoles();
  pRef_ = molr1*pRef_ + molr2*ipg.pRef_;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator-=
(
  const incompressiblePerfectGas<Specie>& ipg
)
{
  Specie::operator-=(ipg);
  pRef_ = ipg.pRef_;
}
template<class Specie>
inline void mousse::incompressiblePerfectGas<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator+
(
  const incompressiblePerfectGas<Specie>& ipg1,
  const incompressiblePerfectGas<Specie>& ipg2
)
{
  scalar nMoles = ipg1.nMoles() + ipg2.nMoles();
  scalar molr1 = ipg1.nMoles()/nMoles;
  scalar molr2 = ipg2.nMoles()/nMoles;
  return {static_cast<const Specie&>(ipg1) + static_cast<const Specie&>(ipg2),
          molr1*ipg1.pRef_ + molr2*ipg2.pRef_};
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator-
(
  const incompressiblePerfectGas<Specie>& ipg1,
  const incompressiblePerfectGas<Specie>& ipg2
)
{
  return {static_cast<const Specie&>(ipg1) - static_cast<const Specie&>(ipg2),
          ipg1.pRef_};
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator*
(
  const scalar s,
  const incompressiblePerfectGas<Specie>& ipg
)
{
  return {s*static_cast<const Specie&>(ipg), ipg.pRef_};
}
template<class Specie>
inline mousse::incompressiblePerfectGas<Specie> mousse::operator==
(
  const incompressiblePerfectGas<Specie>& pg1,
  const incompressiblePerfectGas<Specie>& pg2
)
{
  return pg2 - pg1;
}
#ifdef NoRepository
#   include "incompressible_perfect_gas.cpp"
#endif
#endif
