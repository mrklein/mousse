// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::perfectGas
// Description
//   Perfect gas equation of state.
// SourceFiles
//   perfect_gas_i.hpp
//   perfect_gas.cpp
#ifndef perfect_gas_hpp_
#define perfect_gas_hpp_
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
#include "perfect_gas_i.hpp"
#ifdef NoRepository
#   include "perfect_gas.cpp"
#endif
#endif
