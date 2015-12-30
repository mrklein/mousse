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
//   linear_i.hpp
//   linear.cpp
#ifndef linear_hpp_
#define linear_hpp_
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
#include "linear_i.hpp"
#ifdef NoRepository
#   include "linear.cpp"
#endif
#endif
