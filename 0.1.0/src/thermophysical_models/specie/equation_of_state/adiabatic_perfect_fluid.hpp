// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adiabaticPerfectFluid
// Description
//   AdiabaticPerfect gas equation of state.
// SourceFiles
//   adiabatic_perfect_fluid_i.hpp
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
#include "adiabatic_perfect_fluid_i.hpp"
#ifdef NoRepository
#   include "adiabatic_perfect_fluid.cpp"
#endif
#endif
