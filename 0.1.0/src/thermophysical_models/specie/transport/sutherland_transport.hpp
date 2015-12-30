// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sutherlandTransport
// Description
//   Transport package using Sutherland's formula.
//   Templated into a given thermodynamics package (needed for thermal
//   conductivity).
//   Dynamic viscosity [kg/m.s]
//   \f[
//     \mu = A_s \frac{\sqrt{T}}{1 + T_s / T}
//   \f]
// SourceFiles
//   sutherland_transport_i.hpp
//   sutherland_transport.cpp
#ifndef sutherland_transport_hpp_
#define sutherland_transport_hpp_
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo> class sutherlandTransport;
template<class Thermo>
inline sutherlandTransport<Thermo> operator+
(
  const sutherlandTransport<Thermo>&,
  const sutherlandTransport<Thermo>&
);
template<class Thermo>
inline sutherlandTransport<Thermo> operator-
(
  const sutherlandTransport<Thermo>&,
  const sutherlandTransport<Thermo>&
);
template<class Thermo>
inline sutherlandTransport<Thermo> operator*
(
  const scalar,
  const sutherlandTransport<Thermo>&
);
template<class Thermo>
inline sutherlandTransport<Thermo> operator==
(
  const sutherlandTransport<Thermo>&,
  const sutherlandTransport<Thermo>&
);
template<class Thermo>
Ostream& operator<<
(
  Ostream&,
  const sutherlandTransport<Thermo>&
);
template<class Thermo>
class sutherlandTransport
:
  public Thermo
{
  // Private data
    // Sutherland's coefficients
    scalar As_, Ts_;
  // Private Member Functions
    //- Calculate the Sutherland coefficients
    //  given two viscosities and temperatures
    inline void calcCoeffs
    (
      const scalar mu1, const scalar T1,
      const scalar mu2, const scalar T2
    );
public:
  // Constructors
    //- Construct from components
    inline sutherlandTransport
    (
      const Thermo& t,
      const scalar As,
      const scalar Ts
    );
    //- Construct from two viscosities
    inline sutherlandTransport
    (
      const Thermo& t,
      const scalar mu1, const scalar T1,
      const scalar mu2, const scalar T2
    );
    //- Construct as named copy
    inline sutherlandTransport(const word&, const sutherlandTransport&);
    //- Construct from Istream
    sutherlandTransport(Istream&);
    //- Construct from dictionary
    sutherlandTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<sutherlandTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<sutherlandTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<sutherlandTransport> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "sutherland<" + Thermo::typeName() + '>';
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
    inline sutherlandTransport& operator=(const sutherlandTransport&);
    inline void operator+=(const sutherlandTransport&);
    inline void operator-=(const sutherlandTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend sutherlandTransport operator+ <Thermo>
    (
      const sutherlandTransport&,
      const sutherlandTransport&
    );
    friend sutherlandTransport operator- <Thermo>
    (
      const sutherlandTransport&,
      const sutherlandTransport&
    );
    friend sutherlandTransport operator* <Thermo>
    (
      const scalar,
      const sutherlandTransport&
    );
    friend sutherlandTransport operator== <Thermo>
    (
      const sutherlandTransport&,
      const sutherlandTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
      Ostream&,
      const sutherlandTransport&
    );
};
}  // namespace mousse
#include "sutherland_transport_i.hpp"
#ifdef NoRepository
#   include "sutherland_transport.cpp"
#endif
#endif
