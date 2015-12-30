// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::icoPolynomial
// Description
//   Incompressible, polynomial form of equation of state, using a polynomial
//   function for density.
// SourceFiles
//   ico_polynomial_i.hpp
//   ico_polynomial.cpp
#ifndef ico_polynomial_hpp_
#define ico_polynomial_hpp_
#include "auto_ptr.hpp"
#include "polynomial.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie, int PolySize>
class icoPolynomial;
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator+
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator-
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator*
(
  const scalar,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator==
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize=8>
class icoPolynomial
:
  public Specie
{
  // Private data
    //- Density polynomial coefficients
    //  Note: input in [kg/m3], but internally uses [kg/m3/kmol]
    Polynomial<PolySize> rhoCoeffs_;
public:
  // Constructors
    //- Construct from components
    inline icoPolynomial
    (
      const Specie& sp,
      const Polynomial<PolySize>& rhoPoly
    );
    //- Construct from Istream
    icoPolynomial(Istream&);
    //- Construct from dictionary
    icoPolynomial(const dictionary& dict);
    //- Construct as copy
    inline icoPolynomial(const icoPolynomial&);
    //- Construct as named copy
    inline icoPolynomial(const word& name, const icoPolynomial&);
    //- Construct and return a clone
    inline autoPtr<icoPolynomial> clone() const;
    // Selector from Istream
    inline static autoPtr<icoPolynomial> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<icoPolynomial> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "icoPolynomial<" + word(Specie::typeName_()) + '>';
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
    inline icoPolynomial& operator=(const icoPolynomial&);
    inline void operator+=(const icoPolynomial&);
    inline void operator-=(const icoPolynomial&);
    inline void operator*=(const scalar);
  // Friend operators
    friend icoPolynomial operator+ <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
    friend icoPolynomial operator- <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
    friend icoPolynomial operator* <Specie, PolySize>
    (
      const scalar s,
      const icoPolynomial&
    );
    friend icoPolynomial operator== <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie, PolySize>
    (
      Ostream&,
      const icoPolynomial&
    );
};
}  // namespace mousse
#define makeIcoPolynomial(PolySize)                                          \
                                      \
defineTemplateTypeNameAndDebugWithName                                       \
(                                                                            \
  icoPolynomial<Specie, PolySize>,                                         \
  "icoPolynomial<"#PolySize">",                                            \
  0                                                                        \
);
#include "ico_polynomial_i.hpp"
#ifdef NoRepository
#   include "ico_polynomial.cpp"
#endif
#endif
