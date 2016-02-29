#ifndef THERMOPHYSICAL_MODELS_SPECIE_TRANSPORT_POLYNOMIAL_TRANSPORT_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_TRANSPORT_POLYNOMIAL_TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polynomialTransport
// Description
//   Transport package using polynomial functions for mu and kappa
// SourceFiles
//   polynomial_transport.cpp
#include "polynomial.hpp"
#include "specie.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo, int PolySize> class polynomialTransport;
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator+
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator-
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator*
(
  const scalar,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator==
(
  const polynomialTransport<Thermo, PolySize>&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const polynomialTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize=8>
class polynomialTransport
:
  public Thermo
{
  // Private data
    //- Dynamic viscosity polynomial coefficients
    //  Note: input in [Pa.s], but internally uses [Pa.s/kmol]
    Polynomial<PolySize> muCoeffs_;
    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K], but internally uses [W/m/K/kmol]
    Polynomial<PolySize> kappaCoeffs_;
  // Private Member Functions
    //- Construct from components
    inline polynomialTransport
    (
      const Thermo& t,
      const Polynomial<PolySize>& muPoly,
      const Polynomial<PolySize>& kappaPoly
    );
public:
  // Constructors
    //- Construct copy
    inline polynomialTransport(const polynomialTransport&);
    //- Construct as named copy
    inline polynomialTransport(const word&, const polynomialTransport&);
    //- Construct from Istream
    polynomialTransport(Istream& is);
    //- Construct from dictionary
    polynomialTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<polynomialTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<polynomialTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<polynomialTransport> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "polynomial<" + Thermo::typeName() + '>';
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
    inline polynomialTransport& operator=(const polynomialTransport&);
    inline void operator+=(const polynomialTransport&);
    inline void operator-=(const polynomialTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend polynomialTransport operator+ <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
    friend polynomialTransport operator- <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
    friend polynomialTransport operator* <Thermo, PolySize>
    (
      const scalar,
      const polynomialTransport&
    );
    friend polynomialTransport operator== <Thermo, PolySize>
    (
      const polynomialTransport&,
      const polynomialTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
      Ostream&,
      const polynomialTransport&
    );
};
}  // namespace mousse

// Constructors 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const polynomialTransport& pt
)
:
  Thermo{pt},
  muCoeffs_{pt.muCoeffs_},
  kappaCoeffs_{pt.kappaCoeffs_}
{}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const Thermo& t,
  const Polynomial<PolySize>& muCoeffs,
  const Polynomial<PolySize>& kappaCoeffs
)
:
  Thermo{t},
  muCoeffs_{muCoeffs},
  kappaCoeffs_{kappaCoeffs}
{}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const word& name,
  const polynomialTransport& pt
)
:
  Thermo{name, pt},
  muCoeffs_{pt.muCoeffs_},
  kappaCoeffs_{pt.kappaCoeffs_}
{}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize>>
mousse::polynomialTransport<Thermo, PolySize>::clone() const
{
  return {new polynomialTransport<Thermo, PolySize>{*this}};
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize>>
mousse::polynomialTransport<Thermo, PolySize>::New(Istream& is)
{
  return {new polynomialTransport<Thermo, PolySize>(is)};
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialTransport<Thermo, PolySize>>
mousse::polynomialTransport<Thermo, PolySize>::New(const dictionary& dict)
{
  return autoPtr<polynomialTransport<Thermo, PolySize>>
         {
           new polynomialTransport<Thermo, PolySize>(dict)
         };
}
// Member Functions 
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::mu
(
  const scalar /*p*/,
  const scalar T
) const
{
  return muCoeffs_.value(T)/this->W();
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  return kappaCoeffs_.value(T)/this->W();
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialTransport<Thermo, PolySize>::alphah
(
  const scalar p, const scalar T
) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize>&
mousse::polynomialTransport<Thermo, PolySize>::operator=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  Thermo::operator=(pt);
  muCoeffs_ = pt.muCoeffs_;
  kappaCoeffs_ = pt.kappaCoeffs_;
  return *this;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator+=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  muCoeffs_ = molr1*muCoeffs_ + molr2*pt.muCoeffs_;
  kappaCoeffs_ = molr1*kappaCoeffs_ + molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator-=
(
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  muCoeffs_ = molr1*muCoeffs_ - molr2*pt.muCoeffs_;
  kappaCoeffs_ = molr1*kappaCoeffs_ - molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialTransport<Thermo, PolySize>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator+
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return {t,
          molr1*pt1.muCoeffs_ + molr2*pt2.muCoeffs_,
          molr1*pt1.kappaCoeffs_ + molr2*pt2.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator-
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return {t,
          molr1*pt1.muCoeffs_ - molr2*pt2.muCoeffs_,
          molr1*pt1.kappaCoeffs_ - molr2*pt2.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator*
(
  const scalar s,
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  return {s*static_cast<const Thermo&>(pt),
          pt.muCoeffs_,
          pt.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialTransport<Thermo, PolySize> mousse::operator==
(
  const polynomialTransport<Thermo, PolySize>& pt1,
  const polynomialTransport<Thermo, PolySize>& pt2
)
{
  return pt2 - pt1;
}
#ifdef NoRepository
#include "polynomial_transport.cpp"
#endif
#endif
