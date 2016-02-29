#ifndef CORE_PRIMITIVES_COMPLEX_COMPLEX_HPP_
#define CORE_PRIMITIVES_COMPLEX_COMPLEX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::complex
// Description
//   Extension to the c++ complex library type.
// SourceFiles
//   complex.cpp
#include "scalar.hpp"
#include "bool.hpp"
#include "word.hpp"
#include "contiguous.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class complex;
inline scalar magSqr(const complex&);
inline complex sqr(const complex&);
inline scalar mag(const complex&);
inline const complex& max(const complex&, const complex&);
inline const complex& min(const complex&, const complex&);
inline complex limit(const complex&, const complex&);
inline const complex& sum(const complex&);
inline complex operator+(const complex&, const complex&);
inline complex operator-(const complex&);
inline complex operator-(const complex&, const complex&);
inline complex operator*(const complex&, const complex&);
inline complex operator/(const complex&, const complex&);
inline complex operator*(const scalar, const complex&);
inline complex operator*(const complex&, const scalar);
inline complex operator/(const complex&, const scalar);
inline complex operator/(const scalar, const complex&);
Istream& operator>>(Istream&, complex&);
Ostream& operator<<(Ostream&, const complex&);
class complex
{
  // private data
    //- Real and imaginary parts of the complex number
    scalar re, im;
public:
  //- Component type
  typedef complex cmptType;
  // Static data members
    static const char* const typeName;
    static const complex zero;
    static const complex one;
  // Constructors
    //- Construct null
    inline complex();
    //- Construct given real and imaginary parts
    inline complex(const scalar Re, const scalar Im);
    //- Construct from Istream
    complex(Istream&);
  // Member functions
     // Access
       inline scalar Re() const;
       inline scalar Im() const;
     // Edit
       inline scalar& Re();
       inline scalar& Im();
     // Operators
       inline complex conjugate() const;
  // Member operators
    inline const complex& operator=(const complex&);
    inline void operator+=(const complex&);
    inline void operator-=(const complex&);
    inline void operator*=(const complex&);
    inline void operator/=(const complex&);
    inline const complex& operator=(const scalar);
    inline void operator+=(const scalar);
    inline void operator-=(const scalar);
    inline void operator*=(const scalar);
    inline void operator/=(const scalar);
    inline complex operator!() const;
    inline bool operator==(const complex&) const;
    inline bool operator!=(const complex&) const;
  // Friend functions
    friend scalar magSqr(const complex& c);
    friend complex sqr(const complex& c);
    friend scalar mag(const complex& c);
    friend const complex& max(const complex&, const complex&);
    friend const complex& min(const complex&, const complex&);
    friend complex limit(const complex&, const complex&);
    friend const complex& sum(const complex&);
  // Friend operators
    friend complex operator+(const complex&, const complex&);
    friend complex operator-(const complex&);
    friend complex operator-(const complex&, const complex&);
    friend complex operator*(const complex&, const complex&);
    friend complex operator/(const complex&, const complex&);
    friend complex operator*(const scalar, const complex&);
    friend complex operator*(const complex&, const scalar);
    friend complex operator/(const complex&, const scalar);
    friend complex operator/(const scalar, const complex&);
  // IOstream operators
    friend Istream& operator>>(Istream&, complex&);
    friend Ostream& operator<<(Ostream&, const complex&);
};
// Global functions 
//- Return a string representation of a complex
word name(const complex&);
//- Data associated with complex type are contiguous
template<>
inline bool contiguous<complex>() {return true;}
}  // namespace mousse

namespace mousse
{
// Constructors 
inline complex::complex()
{}
inline complex::complex(const scalar Re, const scalar Im)
:
  re{Re},
  im{Im}
{}
// Member Functions 
inline scalar complex::Re() const
{
  return re;
}
inline scalar complex::Im() const
{
  return im;
}
inline scalar& complex::Re()
{
  return re;
}
inline scalar& complex::Im()
{
  return im;
}
inline complex complex::conjugate() const
{
  return {re, -im};
}
// Member Operators 
inline const complex& complex::operator=(const complex& c)
{
  re = c.re;
  im = c.im;
  return *this;
}
inline void complex::operator+=(const complex& c)
{
  re += c.re;
  im += c.im;
}
inline void complex::operator-=(const complex& c)
{
  re -= c.re;
  im -= c.im;
}
inline void complex::operator*=(const complex& c)
{
  *this = (*this)*c;
}
inline void complex::operator/=(const complex& c)
{
  *this = *this/c;
}
inline const complex& complex::operator=(const scalar s)
{
  re = s;
  im = 0.0;
  return *this;
}
inline void complex::operator+=(const scalar s)
{
  re += s;
}
inline void complex::operator-=(const scalar s)
{
  re -= s;
}
inline void complex::operator*=(const scalar s)
{
  re *= s;
  im *= s;
}
inline void complex::operator/=(const scalar s)
{
  re /= s;
  im /= s;
}
inline complex complex::operator!() const
{
  return conjugate();
}
inline bool complex::operator==(const complex& c) const
{
  return (equal(re, c.re) && equal(im, c.im));
}
inline bool complex::operator!=(const complex& c) const
{
  return !operator==(c);
}
// Friend Functions 
inline scalar magSqr(const complex& c)
{
  return (c.re*c.re + c.im*c.im);
}
inline complex sqr(const complex& c)
{
  return c*c;
}
inline scalar mag(const complex& c)
{
  return sqrt(magSqr(c));
}
inline const complex& max(const complex& c1, const complex& c2)
{
  if (mag(c1) > mag(c2))
  {
    return c1;
  }
  else
  {
    return c2;
  }
}
inline const complex& min(const complex& c1, const complex& c2)
{
  if (mag(c1) < mag(c2))
  {
    return c1;
  }
  else
  {
    return c2;
  }
}
inline complex limit(const complex& c1, const complex& c2)
{
  return {limit(c1.re, c2.re), limit(c1.im, c2.im)};
}
inline const complex& sum(const complex& c)
{
  return c;
}
template<class Cmpt>
class Tensor;
inline complex transform(const Tensor<scalar>&, const complex c)
{
  return c;
}
// Friend Operators 
inline complex operator+(const complex& c1, const complex& c2)
{
  return {c1.re + c2.re,
          c1.im + c2.im};
}
inline complex operator-(const complex& c)
{
  return {-c.re,
          -c.im};
}
inline complex operator-(const complex& c1, const complex& c2)
{
  return {c1.re - c2.re,
          c1.im - c2.im};
}
inline complex operator*(const complex& c1, const complex& c2)
{
  return {c1.re*c2.re - c1.im*c2.im,
          c1.im*c2.re + c1.re*c2.im};
}
inline complex operator/(const complex& c1, const complex& c2)
{
  scalar sqrC2 = magSqr(c2);
  return {(c1.re*c2.re + c1.im*c2.im)/sqrC2,
          (c1.im*c2.re - c1.re*c2.im)/sqrC2};
}
inline complex operator*(const scalar s, const complex& c)
{
  return {s*c.re, s*c.im};
}
inline complex operator*(const complex& c, const scalar s)
{
  return {s*c.re, s*c.im};
}
inline complex operator/(const complex& c, const scalar s)
{
  return {c.re/s, c.im/s};
}
inline complex operator/(const scalar s, const complex& c)
{
  return {s/c.re, s/c.im};
}
}  // namespace mousse
#endif
