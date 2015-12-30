// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constIsoSolidTransport
// Description
//   Constant properties Transport package.
//   Templated into a given thermodynamics package (needed for thermal
//   conductivity).
// SourceFiles
//   const_iso_solid_transport_i.hpp
//   const_iso_solid_transport.cpp
#ifndef const_iso_solid_transport_hpp_
#define const_iso_solid_transport_hpp_
#include "vector.hpp"
namespace mousse
{
template<class Thermo> class constIsoSolidTransport;
template<class Thermo>
inline constIsoSolidTransport<Thermo> operator*
(
  const scalar,
  const constIsoSolidTransport<Thermo>&
);
template<class Thermo>
Ostream& operator<<
(
  Ostream&,
  const constIsoSolidTransport<Thermo>&
);
template<class Thermo>
class constIsoSolidTransport
:
  public Thermo
{
  // Private data
    //- Constant isotropic thermal conductivity
    scalar kappa_;
  // Private Member Functions
    //- Construct from components
    inline constIsoSolidTransport(const Thermo& t, const scalar kappa);
public:
  // Constructors
    //- Construct as named copy
    inline constIsoSolidTransport
    (
      const word&,
      const constIsoSolidTransport&
    );
    //- Construct from Istream
    constIsoSolidTransport(const dictionary& dict);
    // Selector from dictionary
    inline static autoPtr<constIsoSolidTransport> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "constIso<" + Thermo::typeName() + '>';
    }
    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;
    //- Isotropic thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Un-isotropic thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const;
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline constIsoSolidTransport& operator=
    (
      const constIsoSolidTransport&
    );
    inline void operator+=(const constIsoSolidTransport&);
    inline void operator-=(const constIsoSolidTransport&);
  // Friend operators
    friend constIsoSolidTransport operator* <Thermo>
    (
      const scalar,
      const constIsoSolidTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
      Ostream&,
      const constIsoSolidTransport&
    );
};
}  // namespace mousse
#include "const_iso_solid_transport_i.hpp"
#ifdef NoRepository
#   include "const_iso_solid_transport.cpp"
#endif
#endif
