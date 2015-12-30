// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constTransport
// Description
//   Constant properties Transport package.
//   Templated into a given thermodynamics package (needed for thermal
//   conductivity).
// SourceFiles
//   const_transport_i.hpp
//   const_transport.cpp
#ifndef const_transport_hpp_
#define const_transport_hpp_
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo> class constTransport;
template<class Thermo>
inline constTransport<Thermo> operator+
(
  const constTransport<Thermo>&,
  const constTransport<Thermo>&
);
template<class Thermo>
inline constTransport<Thermo> operator-
(
  const constTransport<Thermo>&,
  const constTransport<Thermo>&
);
template<class Thermo>
inline constTransport<Thermo> operator*
(
  const scalar,
  const constTransport<Thermo>&
);
template<class Thermo>
inline constTransport<Thermo> operator==
(
  const constTransport<Thermo>&,
  const constTransport<Thermo>&
);
template<class Thermo>
Ostream& operator<<
(
  Ostream&,
  const constTransport<Thermo>&
);
template<class Thermo>
class constTransport
:
  public Thermo
{
  // Private data
    //- Constant dynamic viscosity [Pa.s]
    scalar mu_;
    //- Reciprocal Prandtl Number []
    scalar rPr_;
  // Private Member Functions
    //- Construct from components
    inline constTransport
    (
      const Thermo& t,
      const scalar mu,
      const scalar Pr
    );
public:
  // Constructors
    //- Construct as named copy
    inline constTransport(const word&, const constTransport&);
    //- Construct from Istream
    constTransport(Istream&);
    //- Construct from dictionary
    constTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<constTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<constTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<constTransport> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "const<" + Thermo::typeName() + '>';
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
    inline constTransport& operator=(const constTransport&);
    inline void operator+=(const constTransport&);
    inline void operator-=(const constTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend constTransport operator+ <Thermo>
    (
      const constTransport&,
      const constTransport&
    );
    friend constTransport operator- <Thermo>
    (
      const constTransport&,
      const constTransport&
    );
    friend constTransport operator* <Thermo>
    (
      const scalar,
      const constTransport&
    );
    friend constTransport operator== <Thermo>
    (
      const constTransport&,
      const constTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
      Ostream&,
      const constTransport&
    );
};
}  // namespace mousse
#include "const_transport_i.hpp"
#ifdef NoRepository
#   include "const_transport.cpp"
#endif
#endif
