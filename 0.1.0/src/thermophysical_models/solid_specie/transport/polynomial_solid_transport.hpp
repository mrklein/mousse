// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polynomialSolidTransport
// Description
//   Transport package using polynomial functions for solid kappa
// SourceFiles
//   polynomial_solid_transport_i.hpp
//   polynomial_solid_transport.cpp
#ifndef polynomial_solid_transport_hpp_
#define polynomial_solid_transport_hpp_
#include "polynomial.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo, int PolySize> class polynomialSolidTransport;
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator+
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator-
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator*
(
  const scalar,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator==
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize=8>
class polynomialSolidTransport
:
  public Thermo
{
  // Private data
    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K]
    Polynomial<PolySize> kappaCoeffs_;
  // Private Member Functions
    //- Construct from components
    inline polynomialSolidTransport
    (
      const Thermo& t,
      const Polynomial<PolySize>& kappaPoly
    );
public:
  // Constructors
    //- Construct copy
    inline polynomialSolidTransport(const polynomialSolidTransport&);
    //- Construct as named copy
    inline polynomialSolidTransport
    (
      const word&,
      const polynomialSolidTransport&
    );
    //- Construct from Istream
    polynomialSolidTransport(Istream& is);
    //- Construct from dictionary
    polynomialSolidTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<polynomialSolidTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<polynomialSolidTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<polynomialSolidTransport> New
    (
      const dictionary&dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "polynomial<" + Thermo::typeName() + '>';
    }
    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline polynomialSolidTransport& operator=
    (
      const polynomialSolidTransport&
    );
    inline void operator+=(const polynomialSolidTransport&);
    inline void operator-=(const polynomialSolidTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend polynomialSolidTransport operator+ <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator- <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator* <Thermo, PolySize>
    (
      const scalar,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator== <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
      Ostream&,
      const polynomialSolidTransport&
    );
};
}  // namespace mousse
#include "polynomial_solid_transport_i.hpp"
#ifdef NoRepository
#   include "polynomial_solid_transport.cpp"
#endif
#endif
