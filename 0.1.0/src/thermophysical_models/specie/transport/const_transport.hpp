#ifndef THERMOPHYSICAL_MODELS_SPECIE_TRANSPORT_CONST_TRANSPORT_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_TRANSPORT_CONST_TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constTransport
// Description
//   Constant properties Transport package.
//   Templated into a given thermodynamics package (needed for thermal
//   conductivity).

namespace mousse {

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


// Constructors 
template<class Thermo>
inline mousse::constTransport<Thermo>::constTransport
(
  const Thermo& t,
  const scalar mu,
  const scalar Pr
)
:
  Thermo{t},
  mu_{mu},
  rPr_{1.0/Pr}
{}


template<class Thermo>
inline mousse::constTransport<Thermo>::constTransport
(
  const word& name,
  const constTransport& ct
)
:
  Thermo{name, ct},
  mu_{ct.mu_},
  rPr_{ct.rPr_}
{}


template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo>>
mousse::constTransport<Thermo>::clone() const
{
  return {new constTransport<Thermo>{*this}};
}


template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo>>
mousse::constTransport<Thermo>::New
(
  Istream& is
)
{
  return {new constTransport<Thermo>{is}};
}


template<class Thermo>
inline mousse::autoPtr<mousse::constTransport<Thermo>>
mousse::constTransport<Thermo>::New
(
  const dictionary& dict
)
{
  return autoPtr<mousse::constTransport<Thermo>>{new constTransport<Thermo>{dict}};
}


// Member Functions 
template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::mu
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  return mu_;
}


template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::kappa
(
  const scalar p,
  const scalar T
) const
{
  return this->Cp(p, T)*mu(p, T)*rPr_;
}


template<class Thermo>
inline mousse::scalar mousse::constTransport<Thermo>::alphah
(
  const scalar p,
  const scalar T
) const
{
  return mu(p, T)*rPr_;
}


// Member Operators 
template<class Thermo>
inline mousse::constTransport<Thermo>& mousse::constTransport<Thermo>::operator=
(
  const constTransport<Thermo>& ct
)
{
  Thermo::operator=(ct);
  mu_ = ct.mu_;
  rPr_ = ct.rPr_;
  return *this;
}


template<class Thermo>
inline void mousse::constTransport<Thermo>::operator+=
(
  const constTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(st);
  if (mag(molr1) + mag(st.nMoles()) > SMALL) {
    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();
    mu_ = molr1*mu_ + molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ + molr2/st.rPr_);
  }
}


template<class Thermo>
inline void mousse::constTransport<Thermo>::operator-=
(
  const constTransport<Thermo>& st
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(st);
  if (mag(molr1) + mag(st.nMoles()) > SMALL) {
    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();
    mu_ = molr1*mu_ - molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ - molr2/st.rPr_);
  }
}


template<class Thermo>
inline void mousse::constTransport<Thermo>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}


// Friend Operators 
template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator+
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  Thermo t{static_cast<const Thermo&>(ct1) + static_cast<const Thermo&>(ct2)};
  if (mag(ct1.nMoles()) + mag(ct2.nMoles()) < SMALL) {
    return
      constTransport<Thermo>
      {
        t,
        0,
        ct1.rPr_
      };
  } else {
    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();
    return
      constTransport<Thermo>
      {
        t,
        molr1*ct1.mu_ + molr2*ct2.mu_,
        1.0/(molr1/ct1.rPr_ + molr2/ct2.rPr_)
      };
  }
}


template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator-
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  Thermo t{static_cast<const Thermo&>(ct1) - static_cast<const Thermo&>(ct2)};
  if (mag(ct1.nMoles()) + mag(ct2.nMoles()) < SMALL) {
    return
      constTransport<Thermo>
      {
        t,
        0,
        ct1.rPr_
      };
  } else {
    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();
    return
      constTransport<Thermo>
      {
        t,
        molr1*ct1.mu_ - molr2*ct2.mu_,
        1.0/(molr1/ct1.rPr_ - molr2/ct2.rPr_)
      };
  }
}


template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator*
(
  const scalar s,
  const constTransport<Thermo>& ct
)
{
  return
    constTransport<Thermo>
    {
      s*static_cast<const Thermo&>(ct),
      ct.mu_,
      1.0/ct.rPr_
    };
}


template<class Thermo>
inline mousse::constTransport<Thermo> mousse::operator==
(
  const constTransport<Thermo>& ct1,
  const constTransport<Thermo>& ct2
)
{
  return ct2 - ct1;
}

#include "const_transport.ipp"

#endif
