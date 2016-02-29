#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_CONST_AN_ISO_SOLID_TRANSPORT_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_CONST_AN_ISO_SOLID_TRANSPORT_HPP_

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

// Constructors 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
  const Thermo& t,
  const vector kappa
)
:
  Thermo{t},
  kappa_{kappa}
{}
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
  const word& name,
  const constAnIsoSolidTransport& ct
)
:
  Thermo{name, ct},
  kappa_{ct.kappa_}
{}
template<class Thermo>
inline mousse::autoPtr<mousse::constAnIsoSolidTransport<Thermo> >
mousse::constAnIsoSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<constAnIsoSolidTransport<Thermo> >
  {
    new constAnIsoSolidTransport<Thermo>{dict}
  };
}
// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::constAnIsoSolidTransport<Thermo>::
kappa(const scalar /*p*/, const scalar /*T*/) const
{
  return mag(kappa_);
}
template<class Thermo>
inline mousse::vector mousse::constAnIsoSolidTransport<Thermo>::
Kappa(const scalar /*p*/, const scalar /*T*/) const
{
  return kappa_;
}
template<class Thermo>
inline mousse::scalar mousse::constAnIsoSolidTransport<Thermo>::
mu(const scalar /*p*/, const scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::constAnIsoSolidTransport<Thermo>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}
template<class Thermo>
inline mousse::vector mousse::constAnIsoSolidTransport<Thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa_/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo>&
mousse::constAnIsoSolidTransport<Thermo>::operator=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  kappa_ = ct.kappa_;
  return *this;
}
template<class Thermo>
inline void mousse::constAnIsoSolidTransport<Thermo>::operator+=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ + molr2*ct.kappa_;
}
template<class Thermo>
inline void mousse::constAnIsoSolidTransport<Thermo>::operator-=
(
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ - molr2*ct.kappa_;
}
// Friend Operators 
template<class Thermo>
inline mousse::constAnIsoSolidTransport<Thermo> mousse::operator*
(
  const scalar s,
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  return {s*static_cast<const Thermo&>(ct), ct.kappa_};
}
#ifdef NoRepository
#   include "const_an_iso_solid_transport.cpp"
#endif
#endif
