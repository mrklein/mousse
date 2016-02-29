#ifndef THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_PERFECT_GAS_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_EQUATION_OF_STATE_PERFECT_GAS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::perfectGas
// Description
//   Perfect gas equation of state.
// SourceFiles
//   perfect_gas.cpp
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class perfectGas;
template<class Specie>
inline perfectGas<Specie> operator+
(
  const perfectGas<Specie>&,
  const perfectGas<Specie>&
);
template<class Specie>
inline perfectGas<Specie> operator-
(
  const perfectGas<Specie>&,
  const perfectGas<Specie>&
);
template<class Specie>
inline perfectGas<Specie> operator*
(
  const scalar,
  const perfectGas<Specie>&
);
template<class Specie>
inline perfectGas<Specie> operator==
(
  const perfectGas<Specie>&,
  const perfectGas<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const perfectGas<Specie>&
);
template<class Specie>
class perfectGas
:
  public Specie
{
public:
  // Constructors
    //- Construct from components
    inline perfectGas(const Specie& sp);
    //- Construct from Istream
    perfectGas(Istream&);
    //- Construct from dictionary
    perfectGas(const dictionary& dict);
    //- Construct as named copy
    inline perfectGas(const word& name, const perfectGas&);
    //- Construct and return a clone
    inline autoPtr<perfectGas> clone() const;
    // Selector from Istream
    inline static autoPtr<perfectGas> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<perfectGas> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "perfectGas<" + word(Specie::typeName_()) + '>';
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
    inline void operator+=(const perfectGas&);
    inline void operator-=(const perfectGas&);
    inline void operator*=(const scalar);
  // Friend operators
    friend perfectGas operator+ <Specie>
    (
      const perfectGas&,
      const perfectGas&
    );
    friend perfectGas operator- <Specie>
    (
      const perfectGas&,
      const perfectGas&
    );
    friend perfectGas operator* <Specie>
    (
      const scalar s,
      const perfectGas&
    );
    friend perfectGas operator== <Specie>
    (
      const perfectGas&,
      const perfectGas&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const perfectGas&
    );
};
}  // namespace mousse

// Private Member Functions 
template<class Specie>
inline mousse::perfectGas<Specie>::perfectGas(const Specie& sp)
:
  Specie{sp}
{}
// Constructors 
template<class Specie>
inline mousse::perfectGas<Specie>::perfectGas
(
  const word& name,
  const perfectGas<Specie>& pg
)
:
  Specie{name, pg}
{}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie>>
mousse::perfectGas<Specie>::clone() const
{
  return {new perfectGas<Specie>{*this}};
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie>>
mousse::perfectGas<Specie>::New(Istream& is)
{
  return {new perfectGas<Specie>{is}};
}
template<class Specie>
inline mousse::autoPtr<mousse::perfectGas<Specie>> mousse::perfectGas<Specie>::New
(
  const dictionary& dict
)
{
  return {new perfectGas<Specie>{dict}};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::rho(scalar p, scalar T) const
{
  return p/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::s(scalar p, scalar /*T*/) const
{
  return -RR*log(p/Pstd);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::psi(scalar /*p*/, scalar T) const
{
  return 1.0/(this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::Z(scalar /*p*/, scalar /*T*/) const
{
  return 1;
}
template<class Specie>
inline mousse::scalar mousse::perfectGas<Specie>::cpMcv(scalar /*p*/, scalar /*T*/) const
{
  return RR;
}
// Member Operators 
template<class Specie>
inline void mousse::perfectGas<Specie>::operator+=(const perfectGas<Specie>& pg)
{
  Specie::operator+=(pg);
}
template<class Specie>
inline void mousse::perfectGas<Specie>::operator-=(const perfectGas<Specie>& pg)
{
  Specie::operator-=(pg);
}
template<class Specie>
inline void mousse::perfectGas<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator+
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return {static_cast<const Specie&>(pg1) + static_cast<const Specie&>(pg2)};
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator-
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return {static_cast<const Specie&>(pg1) - static_cast<const Specie&>(pg2)};
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator*
(
  const scalar s,
  const perfectGas<Specie>& pg
)
{
  return {s*static_cast<const Specie&>(pg)};
}
template<class Specie>
inline mousse::perfectGas<Specie> mousse::operator==
(
  const perfectGas<Specie>& pg1,
  const perfectGas<Specie>& pg2
)
{
  return pg2 - pg1;
}
#ifdef NoRepository
#   include "perfect_gas.cpp"
#endif
#endif
