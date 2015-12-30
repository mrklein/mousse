// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::perfectFluid
// Description
//   Perfect gas equation of state.
// SourceFiles
//   perfect_fluid_i.hpp
//   perfect_fluid.cpp
#ifndef perfect_fluid_hpp_
#define perfect_fluid_hpp_
#include "auto_ptr.hpp"
namespace mousse
{
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
#include "perfect_fluid_i.hpp"
#ifdef NoRepository
#   include "perfect_fluid.cpp"
#endif
#endif
