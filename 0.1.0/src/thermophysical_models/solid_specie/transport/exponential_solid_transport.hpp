// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::exponentialSolidTransport
// Description
//   Exponential properties for solid heat transport
//   Templated into a given thermodynamics package.
// SourceFiles
//   exponential_solid_transport_i.hpp
//   exponential_solid_transport.cpp
#ifndef exponential_solid_transport_hpp_
#define exponential_solid_transport_hpp_
namespace mousse
{
template<class Thermo> class exponentialSolidTransport;
template<class Thermo>
inline exponentialSolidTransport<Thermo> operator*
(
  const scalar,
  const exponentialSolidTransport<Thermo>&
);
template<class Thermo>
Ostream& operator<<
(
  Ostream&,
  const exponentialSolidTransport<Thermo>&
);
template<class Thermo>
class exponentialSolidTransport
:
  public Thermo
{
  // Private data
    //- Constant thermal coefficient.
    scalar kappa0_;
    //- Exponent coefficient
    scalar n0_;
    //- Reference temperature
    scalar Tref_;
  // Private Member Functions
    //- Construct from components
    inline exponentialSolidTransport
    (
      const Thermo& t,
      const scalar kappa0,
      const scalar n0,
      const scalar Tref
    );
public:
  // Constructors
    //- Construct as named copy
    inline exponentialSolidTransport
    (
      const word&,
      const exponentialSolidTransport&
    );
    //- Construct from dictionary
    exponentialSolidTransport(const dictionary&);
    // Selector from dictionary
    inline static autoPtr<exponentialSolidTransport> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "exponential<" + Thermo::typeName() + '>';
    }
    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;
    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const;
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline exponentialSolidTransport& operator=
    (
      const exponentialSolidTransport&
    );
    inline void operator+=(const exponentialSolidTransport&);
    inline void operator-=(const exponentialSolidTransport&);
  // Friend operators
    friend exponentialSolidTransport operator* <Thermo>
    (
      const scalar,
      const exponentialSolidTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
      Ostream&,
      const exponentialSolidTransport&
    );
};
}  // namespace mousse
#include "exponential_solid_transport_i.hpp"
#ifdef NoRepository
#   include "exponential_solid_transport.cpp"
#endif
#endif
