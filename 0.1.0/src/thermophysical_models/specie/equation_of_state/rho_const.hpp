// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoConst
// Description
//   RhoConst (rho = const) of state.
// SourceFiles
//   rho_const_i.hpp
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
#include "rho_const_i.hpp"
#ifdef NoRepository
#   include "rho_const.cpp"
#endif
#endif
