#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_CONST_ISO_SOLID_TRANSPORT_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_CONST_ISO_SOLID_TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constIsoSolidTransport
// Description
//   Constant properties Transport package.
//   Templated into a given thermodynamics package (needed for thermal
//   conductivity).

#include "vector.hpp"


namespace mousse {

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


// Constructors 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo>::constIsoSolidTransport
(
  const thermo& t,
  const scalar kappa
)
:
  thermo{t},
  kappa_{kappa}
{}


template<class thermo>
inline mousse::constIsoSolidTransport<thermo>::constIsoSolidTransport
(
  const word& name,
  const constIsoSolidTransport& ct
)
:
  thermo{name, ct},
  kappa_{ct.kappa_}
{}


template<class Thermo>
inline mousse::autoPtr<mousse::constIsoSolidTransport<Thermo>>
mousse::constIsoSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<constIsoSolidTransport<Thermo>>
  {
    new constIsoSolidTransport<Thermo>{dict}
  };
}


// Member Functions 
template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
kappa(const scalar /*p*/, const scalar /*T*/) const
{
  return kappa_;
}


template<class thermo>
inline mousse::vector mousse::constIsoSolidTransport<thermo>::
Kappa(const scalar /*p*/, const scalar /*T*/) const
{
  return vector(kappa_, kappa_, kappa_);
}


template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
mu(const scalar /*p*/, const scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::constIsoSolidTransport<thermo>mu::"
    "("
    "  const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}


template<class thermo>
inline mousse::scalar mousse::constIsoSolidTransport<thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa_/this->Cpv(p, T);
}


// Member Operators 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo>&
mousse::constIsoSolidTransport<thermo>::operator=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  thermo::operator=(ct);
  kappa_ = ct.kappa_;
  return *this;
}


template<class thermo>
inline void mousse::constIsoSolidTransport<thermo>::operator+=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  thermo::operator+=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ + molr2*ct.kappa_;
}


template<class thermo>
inline void mousse::constIsoSolidTransport<thermo>::operator-=
(
  const constIsoSolidTransport<thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  thermo::operator-=(ct);
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa_ = molr1*kappa_ - molr2*ct.kappa_;
}


// Friend Operators 
template<class thermo>
inline mousse::constIsoSolidTransport<thermo> mousse::operator*
(
  const scalar s,
  const constIsoSolidTransport<thermo>& ct
)
{
  return {s*static_cast<const thermo&>(ct), ct.kappa_};
}

#include "const_iso_solid_transport.ipp"

#endif
