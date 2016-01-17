// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::icoPolynomial
// Description
//   Incompressible, polynomial form of equation of state, using a polynomial
//   function for density.
// SourceFiles
//   ico_polynomial.cpp
#ifndef ico_polynomial_hpp_
#define ico_polynomial_hpp_
#include "auto_ptr.hpp"
#include "polynomial.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie, int PolySize>
class icoPolynomial;
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator+
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator-
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator*
(
  const scalar,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator==
(
  const icoPolynomial<Specie, PolySize>&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize>
Ostream& operator<<
(
  Ostream&,
  const icoPolynomial<Specie, PolySize>&
);
template<class Specie, int PolySize=8>
class icoPolynomial
:
  public Specie
{
  // Private data
    //- Density polynomial coefficients
    //  Note: input in [kg/m3], but internally uses [kg/m3/kmol]
    Polynomial<PolySize> rhoCoeffs_;
public:
  // Constructors
    //- Construct from components
    inline icoPolynomial
    (
      const Specie& sp,
      const Polynomial<PolySize>& rhoPoly
    );
    //- Construct from Istream
    icoPolynomial(Istream&);
    //- Construct from dictionary
    icoPolynomial(const dictionary& dict);
    //- Construct as copy
    inline icoPolynomial(const icoPolynomial&);
    //- Construct as named copy
    inline icoPolynomial(const word& name, const icoPolynomial&);
    //- Construct and return a clone
    inline autoPtr<icoPolynomial> clone() const;
    // Selector from Istream
    inline static autoPtr<icoPolynomial> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<icoPolynomial> New(const dictionary& dict);
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "icoPolynomial<" + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = true;
      //- Is the equation of state is isochoric i.e. rho = const
      static const bool isochoric = false;
      //- Return density [kg/m^3]
      inline scalar rho(scalar p, scalar T) const;
      //- Return entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
      //- Return compressibility rho/p [s^2/m^2]
      inline scalar psi(scalar p, scalar T) const;
      //- Return compression factor []
      inline scalar Z(scalar p, scalar T) const;
      //- Return (cp - cv) [J/(kmol K]
      inline scalar cpMcv(scalar p, scalar T) const;
    // IO
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline icoPolynomial& operator=(const icoPolynomial&);
    inline void operator+=(const icoPolynomial&);
    inline void operator-=(const icoPolynomial&);
    inline void operator*=(const scalar);
  // Friend operators
    friend icoPolynomial operator+ <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
    friend icoPolynomial operator- <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
    friend icoPolynomial operator* <Specie, PolySize>
    (
      const scalar s,
      const icoPolynomial&
    );
    friend icoPolynomial operator== <Specie, PolySize>
    (
      const icoPolynomial&,
      const icoPolynomial&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie, PolySize>
    (
      Ostream&,
      const icoPolynomial&
    );
};
}  // namespace mousse
#define makeIcoPolynomial(PolySize)                                          \
                                      \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUGWithName                                       \
(                                                                            \
  icoPolynomial<Specie, PolySize>,                                         \
  "icoPolynomial<"#PolySize">",                                            \
  0                                                                        \
);

// Private Member Functions 
template<class Specie, int PolySize>
inline mousse::icoPolynomial<Specie, PolySize>::icoPolynomial
(
  const Specie& sp,
  const Polynomial<PolySize>& rhoCoeffs
)
:
  Specie{sp},
  rhoCoeffs_{rhoCoeffs}
{}
// Constructors 
template<class Specie, int PolySize>
inline mousse::icoPolynomial<Specie, PolySize>::icoPolynomial
(
  const icoPolynomial<Specie, PolySize>& ip
)
:
  Specie{ip},
  rhoCoeffs_{ip.rhoCoeffs_}
{}
template<class Specie, int PolySize>
inline mousse::icoPolynomial<Specie, PolySize>::icoPolynomial
(
  const word& name,
  const icoPolynomial<Specie, PolySize>& ip
)
:
  Specie{name, ip},
  rhoCoeffs_{ip.rhoCoeffs_}
{}
template<class Specie, int PolySize>
inline mousse::autoPtr<mousse::icoPolynomial<Specie, PolySize> >
mousse::icoPolynomial<Specie, PolySize>::clone() const
{
  return {new icoPolynomial<Specie, PolySize>(*this)};
}
template<class Specie, int PolySize>
inline mousse::autoPtr<mousse::icoPolynomial<Specie, PolySize> >
mousse::icoPolynomial<Specie, PolySize>::New(Istream& is)
{
  return {new icoPolynomial<Specie, PolySize>(is)};
}
template<class Specie, int PolySize>
inline mousse::autoPtr<mousse::icoPolynomial<Specie, PolySize> >
mousse::icoPolynomial<Specie, PolySize>::New(const dictionary& dict)
{
  return {new icoPolynomial<Specie, PolySize>(dict)};
}
// Member Functions 
template<class Specie, int PolySize>
inline mousse::scalar mousse::icoPolynomial<Specie, PolySize>::rho
(
  scalar /*p*/,
  scalar T
) const
{
  return rhoCoeffs_.value(T)/this->W();
}
template<class Specie, int PolySize>
inline mousse::scalar mousse::icoPolynomial<Specie, PolySize>::s
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie, int PolySize>
inline mousse::scalar mousse::icoPolynomial<Specie, PolySize>::psi
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie, int PolySize>
inline mousse::scalar mousse::icoPolynomial<Specie, PolySize>::Z
(
  scalar /*p*/,
  scalar /*T*/
) const
{
  return 0;
}
template<class Specie, int PolySize>
inline mousse::scalar mousse::icoPolynomial<Specie, PolySize>::cpMcv
(
  scalar p,
  scalar T
) const
{
  return -(p/sqr(rhoCoeffs_.value(T)))*rhoCoeffs_.derivative(T);
}
// Member Operators 
template<class Specie, int PolySize>
inline mousse::icoPolynomial<Specie, PolySize>&
mousse::icoPolynomial<Specie, PolySize>::operator=
(
  const icoPolynomial<Specie, PolySize>& ip
)
{
  Specie::operator=(ip);
  rhoCoeffs_ = ip.rhoCoeffs_;
  return *this;
}
template<class Specie, int PolySize>
inline void mousse::icoPolynomial<Specie, PolySize>::operator+=
(
  const icoPolynomial<Specie, PolySize>& ip
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(ip);
  molr1 /= this->nMoles();
  scalar molr2 = ip.nMoles()/this->nMoles();
  rhoCoeffs_ = molr1*rhoCoeffs_ + molr2*ip.rhoCoeffs_;
}
template<class Specie, int PolySize>
inline void mousse::icoPolynomial<Specie, PolySize>::operator-=
(
  const icoPolynomial<Specie, PolySize>& ip
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(ip);
  molr1 /= this->nMoles();
  scalar molr2 = ip.nMoles()/this->nMoles();
  rhoCoeffs_ = molr1*rhoCoeffs_ - molr2*ip.rhoCoeffs_;
}
template<class Specie, int PolySize>
inline void mousse::icoPolynomial<Specie, PolySize>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie, int PolySize>
mousse::icoPolynomial<Specie, PolySize> mousse::operator+
(
  const icoPolynomial<Specie, PolySize>& ip1,
  const icoPolynomial<Specie, PolySize>& ip2
)
{
  scalar nMoles = ip1.nMoles() + ip2.nMoles();
  scalar molr1 = ip1.nMoles()/nMoles;
  scalar molr2 = ip2.nMoles()/nMoles;
  return {static_cast<const Specie&>(ip1) + static_cast<const Specie&>(ip2),
          molr1*ip1.rhoCoeffs_ + molr2*ip2.rhoCoeffs_};
}
template<class Specie, int PolySize>
mousse::icoPolynomial<Specie, PolySize> mousse::operator-
(
  const icoPolynomial<Specie, PolySize>& ip1,
  const icoPolynomial<Specie, PolySize>& ip2
)
{
  scalar nMoles = ip1.nMoles() + ip2.nMoles();
  scalar molr1 = ip1.nMoles()/nMoles;
  scalar molr2 = ip2.nMoles()/nMoles;
  return {static_cast<const Specie&>(ip1) - static_cast<const Specie&>(ip2),
          molr1*ip1.rhoCoeffs_ - molr2*ip2.rhoCoeffs_};
}
template<class Specie, int PolySize>
mousse::icoPolynomial<Specie, PolySize> mousse::operator*
(
  const scalar s,
  const icoPolynomial<Specie, PolySize>& ip
)
{
  return {s*static_cast<const Specie&>(ip), ip.rhoCoeffs_};
}
template<class Specie, int PolySize>
mousse::icoPolynomial<Specie, PolySize> mousse::operator==
(
  const icoPolynomial<Specie, PolySize>& ip1,
  const icoPolynomial<Specie, PolySize>& ip2
)
{
  return ip2 - ip1;
}
#ifdef NoRepository
#   include "ico_polynomial.cpp"
#endif
#endif
