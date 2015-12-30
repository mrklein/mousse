// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polynomialTransport
// Description
//   Transport package using polynomial functions for mu and kappa
// SourceFiles
//   polynomial_transport_i.hpp
//   polynomial_transport.cpp
#ifndef polynomial_transport_hpp_
#define polynomial_transport_hpp_
#include "polynomial.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo, int PolySize> class polynomialTransport;
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator+
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator-
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator*
(
  const scalar,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator==
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize=8>
class polynomialTransport
:
  public Thermo
{
  // Private data
    //- Dynamic viscosity polynomial coefficients
    //  Note: input in [Pa.s], but internally uses [Pa.s/kmol]
    Polynomial<PolySize> muCoeffs_;
    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K], but internally uses [W/m/K/kmol]
    Polynomial<PolySize> kappaCoeffs_;
  // Private Member Functions
    //- Construct from components
    inline polynomialTransport
    (
      const Thermo& t,
      const Polynomial<PolySize>& muPoly,
      const Polynomial<PolySize>& kappaPoly
    );
public:
  // Constructors
    //- Construct copy
    inline polynomialTransport(const polynomialTransport&);
    //- Construct as named copy
    inline polynomialTransport(const word&, const polynomialTransport&);
    //- Construct from Istream
    polynomialTransport(Istream& is);
    //- Construct from dictionary
    polynomialTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<polynomialTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<polynomialTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<polynomialTransport> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "polynomial<" + Thermo::typeName() + '>';
    }
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;
    // Species diffusivity
    //inline scalar D(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline polynomialTransport& operator=(const polynomialTransport&);
    inline void operator+=(const polynomialTransport&);
    inline void operator-=(const polynomialTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend polynomialTransport operator+ <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
    friend polynomialTransport operator- <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
    friend polynomialTransport operator* <Thermo, PolySize>
    (
      const scalar,
      const polynomialTransport&
    );
    friend polynomialTransport operator== <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
      Ostream&,
      const polynomialTransport&
    );
};
}  // namespace mousse
#include "polynomial_transport_i.hpp"
#ifdef NoRepository
#   include "polynomial_transport.cpp"
#endif
#endif
