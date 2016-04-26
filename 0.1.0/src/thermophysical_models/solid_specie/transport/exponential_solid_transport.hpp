#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_EXPONENTIAL_SOLID_TRANSPORT_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_TRANSPORT_EXPONENTIAL_SOLID_TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::exponentialSolidTransport
// Description
//   Exponential properties for solid heat transport
//   Templated into a given thermodynamics package.

namespace mousse {

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


// Constructors 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
  const Thermo& t,
  const scalar kappa0,
  const scalar n0,
  const scalar Tref
)
:
  Thermo{t},
  kappa0_{kappa0},
  n0_{n0},
  Tref_{Tref}
{}


template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
  const word& name,
  const exponentialSolidTransport& ct
)
:
  Thermo{name, ct},
  kappa0_{ct.kappa0_},
  n0_{ct.n0_},
  Tref_{ct.Tref_}
{}


template<class Thermo>
inline mousse::autoPtr<mousse::exponentialSolidTransport<Thermo>>
mousse::exponentialSolidTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return
    autoPtr<exponentialSolidTransport<Thermo>>
    {
      new exponentialSolidTransport<Thermo>(dict)
    };
}


// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::kappa
(
  const scalar /*p*/, const scalar T
) const
{
  return (kappa0_*pow(T/Tref_, n0_));
}




template<class Thermo>
inline mousse::vector mousse::exponentialSolidTransport<Thermo>::Kappa
(
  const scalar /*p*/, const scalar T
) const
{
  const scalar kappa(kappa0_*pow(T/Tref_, n0_));
  return vector(kappa, kappa, kappa);
}


template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::
mu(const scalar /*p*/, const scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::exponentialSolidTransport<Thermo>mu::"
    "("
    "  const scalar p, const scalar T"
    ") const"
  );
  return scalar(0);
}


template<class Thermo>
inline mousse::scalar mousse::exponentialSolidTransport<Thermo>::
alphah(const scalar p, const scalar T) const
{
  return kappa(p, T)/this->Cpv(p, T);
}


// Member Operators 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo>&
mousse::exponentialSolidTransport<Thermo>::operator=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  kappa0_ = ct.kappa0_;
  n0_ = ct.n0_;
  Tref_ = ct.Tref_;
  return *this;
}


template<class Thermo>
inline void mousse::exponentialSolidTransport<Thermo>::operator+=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa0_ = molr1*kappa0_ + molr2*ct.kappa0_;
  n0_ = (molr1*n0_ + molr2*ct.n0_);
  Tref_ = (molr1*Tref_ + molr2*ct.Tref_);
}


template<class Thermo>
inline void mousse::exponentialSolidTransport<Thermo>::operator-=
(
  const exponentialSolidTransport<Thermo>& ct
)
{
  scalar molr1 = this->nMoles();
  molr1 /= this->nMoles();
  scalar molr2 = ct.nMoles()/this->nMoles();
  kappa0_ = (molr1*kappa0_ - molr2*ct.kappa0_);
  n0_ = (molr1*n0_ - molr2*ct.n0_);
  Tref_ = (molr1*Tref_ - molr2*ct.Tref_);
}


// Friend Operators 
template<class Thermo>
inline mousse::exponentialSolidTransport<Thermo> mousse::operator*
(
  const scalar s,
  const exponentialSolidTransport<Thermo>& ct
)
{
  return
    exponentialSolidTransport<Thermo>
    {
      s*static_cast<const Thermo&>(ct),
      ct.kappa0_,
      ct.n0_,
      ct.Tref_
    };
}

#include "exponential_solid_transport.ipp"

#endif
