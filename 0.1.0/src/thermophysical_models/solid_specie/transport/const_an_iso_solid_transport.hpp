// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constAnIsoSolidTransport
// Description
//   Constant properties Transport package.
//   Templated into a given Thermodynamics package (needed for thermal
//   conductivity).
// SourceFiles
//   const_an_iso_solid_transport_i.hpp
//   const_an_iso_solid_transport.cpp
#ifndef const_an_iso_solid_transport_hpp_
#define const_an_iso_solid_transport_hpp_
#include "vector.hpp"
namespace mousse
{
template<class Thermo> class constAnIsoSolidTransport;
template<class Thermo>
inline constAnIsoSolidTransport<Thermo> operator*
(
  const scalar,
  const constAnIsoSolidTransport<Thermo>&
);
template<class Thermo>
Ostream& operator<<
(
  Ostream&,
  const constAnIsoSolidTransport<Thermo>&
);
template<class Thermo>
class constAnIsoSolidTransport
:
  public Thermo
{
  // Private data
    //- Constant anisotropic thermal conductivity.
    vector kappa_;
  // Private Member Functions
    //- Construct from components
    inline constAnIsoSolidTransport(const Thermo& t, const vector kappa);
public:
  // Constructors
    //- Construct as named copy
    inline constAnIsoSolidTransport
    (
      const word&,
      const constAnIsoSolidTransport&
    );
    //- Construct from dictionary
    constAnIsoSolidTransport(const dictionary&);
    // Selector from dictionary
    inline static autoPtr<constAnIsoSolidTransport> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "constAnIso<" + Thermo::typeName() + '>';
    }
    //- Is the thermal conductivity isotropic
    static const bool isotropic = false;
    //- Isotropic thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Un-isotropic thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const;
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline vector alphah(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline constAnIsoSolidTransport& operator=
    (
      const constAnIsoSolidTransport&
    );
    inline void operator+=(const constAnIsoSolidTransport&);
    inline void operator-=(const constAnIsoSolidTransport&);
    // Friend operators
    friend constAnIsoSolidTransport operator* <Thermo>
    (
      const scalar,
      const constAnIsoSolidTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
      Ostream&,
      const constAnIsoSolidTransport&
    );
};
}  // namespace mousse
#include "const_an_iso_solid_transport_i.hpp"
#ifdef NoRepository
#   include "const_an_iso_solid_transport.cpp"
#endif
#endif
