// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polynomialSolidTransport
// Description
//   Transport package using polynomial functions for solid kappa
// SourceFiles
//   polynomial_solid_transport.cpp
#ifndef polynomial_solid_transport_hpp_
#define polynomial_solid_transport_hpp_
#include "polynomial.hpp"
#include "specie.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Thermo, int PolySize> class polynomialSolidTransport;
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator+
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator-
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator*
(
  const scalar,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator==
(
  const polynomialSolidTransport<Thermo, PolySize>&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const polynomialSolidTransport<Thermo, PolySize>&
);
template<class Thermo, int PolySize=8>
class polynomialSolidTransport
:
  public Thermo
{
  // Private data
    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K]
    Polynomial<PolySize> kappaCoeffs_;
  // Private Member Functions
    //- Construct from components
    inline polynomialSolidTransport
    (
      const Thermo& t,
      const Polynomial<PolySize>& kappaPoly
    );
public:
  // Constructors
    //- Construct copy
    inline polynomialSolidTransport(const polynomialSolidTransport&);
    //- Construct as named copy
    inline polynomialSolidTransport
    (
      const word&,
      const polynomialSolidTransport&
    );
    //- Construct from Istream
    polynomialSolidTransport(Istream& is);
    //- Construct from dictionary
    polynomialSolidTransport(const dictionary& dict);
    //- Construct and return a clone
    inline autoPtr<polynomialSolidTransport> clone() const;
    // Selector from Istream
    inline static autoPtr<polynomialSolidTransport> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<polynomialSolidTransport> New
    (
      const dictionary&dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "polynomial<" + Thermo::typeName() + '>';
    }
    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;
    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;
    //- Thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const;
    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;
    //- Write to Ostream
    void write(Ostream& os) const;
  // Member operators
    inline polynomialSolidTransport& operator=
    (
      const polynomialSolidTransport&
    );
    inline void operator+=(const polynomialSolidTransport&);
    inline void operator-=(const polynomialSolidTransport&);
    inline void operator*=(const scalar);
  // Friend operators
    friend polynomialSolidTransport operator+ <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator- <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator* <Thermo, PolySize>
    (
      const scalar,
      const polynomialSolidTransport&
    );
    friend polynomialSolidTransport operator== <Thermo, PolySize>
    (
      const polynomialSolidTransport&,
      const polynomialSolidTransport&
    );
  // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
      Ostream&,
      const polynomialSolidTransport&
    );
};
}  // namespace mousse

// Constructors 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const polynomialSolidTransport& pt
)
:
  Thermo{pt},
  kappaCoeffs_{pt.kappaCoeffs_}
{}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const Thermo& t,
  const Polynomial<PolySize>& kappaCoeffs
)
:
  Thermo{t},
  kappaCoeffs_{kappaCoeffs}
{}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>::
polynomialSolidTransport
(
  const word& name,
  const polynomialSolidTransport& pt
)
:
  Thermo{name, pt},
  kappaCoeffs_{pt.kappaCoeffs_}
{}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::clone() const
{
  return {new polynomialSolidTransport<Thermo, PolySize>(*this)};
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::New(Istream& is)
{
  return {new polynomialSolidTransport<Thermo, PolySize>(is)};
}
template<class Thermo, int PolySize>
inline mousse::autoPtr<mousse::polynomialSolidTransport<Thermo, PolySize> >
mousse::polynomialSolidTransport<Thermo, PolySize>::New(const dictionary& dict)
{
  return {new polynomialSolidTransport<Thermo, PolySize>(dict)};
}
// Member Functions 
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::mu
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::polynomialSolidTransport<thermo, PolySize>mu::"
    "("
    "    const scalar p, const scalar T"
    ") const"
  );
  return scalar{0};
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  return kappaCoeffs_.value(T);
}
template<class Thermo, int PolySize>
inline mousse::vector mousse::polynomialSolidTransport<Thermo, PolySize>::Kappa
(
  const scalar /*p*/,
  const scalar T
) const
{
  const scalar kappa(kappaCoeffs_.value(T));
  return vector(kappa, kappa, kappa);
}
template<class Thermo, int PolySize>
inline mousse::scalar mousse::polynomialSolidTransport<Thermo, PolySize>::alphah
(
  const scalar p, const scalar T
) const
{
  return kappa(p, T)/this->Cpv(p, T);
}
// Member Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize>&
mousse::polynomialSolidTransport<Thermo, PolySize>::operator=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  Thermo::operator=(pt);
  kappaCoeffs_ = pt.kappaCoeffs_;
  return *this;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator+=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator+=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  kappaCoeffs_ = molr1*kappaCoeffs_ + molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator-=
(
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  scalar molr1 = this->nMoles();
  Thermo::operator-=(pt);
  molr1 /= this->nMoles();
  scalar molr2 = pt.nMoles()/this->nMoles();
  kappaCoeffs_ = molr1*kappaCoeffs_ - molr2*pt.kappaCoeffs_;
}
template<class Thermo, int PolySize>
inline void mousse::polynomialSolidTransport<Thermo, PolySize>::operator*=
(
  const scalar s
)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator+
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return {t, molr1*pt1.kappaCoeffs_ + molr2*pt2.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator-
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  Thermo t
  (
    static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
  );
  scalar molr1 = pt1.nMoles()/t.nMoles();
  scalar molr2 = pt2.nMoles()/t.nMoles();
  return {t, molr1*pt1.kappaCoeffs_ - molr2*pt2.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator*
(
  const scalar s,
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  return {s*static_cast<const Thermo&>(pt), pt.kappaCoeffs_};
}
template<class Thermo, int PolySize>
inline mousse::polynomialSolidTransport<Thermo, PolySize> mousse::operator==
(
  const polynomialSolidTransport<Thermo, PolySize>& pt1,
  const polynomialSolidTransport<Thermo, PolySize>& pt2
)
{
  return pt2 - pt1;
}
#ifdef NoRepository
#   include "polynomial_solid_transport.cpp"
#endif
#endif
