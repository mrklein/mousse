#ifndef CORE_PRIMITIVES_SEPTERNION_SEPTERNION_HPP_
#define CORE_PRIMITIVES_SEPTERNION_SEPTERNION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::septernion
// Description
//   Septernion class used to perform translations and rotations in 3D space.
//   It is composed of a translation vector and rotation quaternion and as
//   such has seven components hence the name "septernion" from the Latin to
//   be consistent with quaternion rather than "hepternion" derived from the
//   Greek.
// SourceFiles
//   septernion.cpp

#include "vector.hpp"
#include "quaternion.hpp"
#include "word.hpp"
#include "contiguous.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class septernion;
Istream& operator>>(Istream& is, septernion&);
Ostream& operator<<(Ostream& os, const septernion& C);


class septernion
{
  // Private data
    //- Translation vector
    vector t_;
    //- Rotation quaternion
    quaternion r_;
public:
  // Static data members
    static const char* const typeName;
    static const septernion zero;
    static const septernion I;
  // Constructors
    //- Construct null
    inline septernion();
    //- Construct given a translation vector and rotation quaternion
    inline septernion(const vector& t, const quaternion& r);
    //- Construct a pure translation septernion given a translation vector
    inline explicit septernion(const vector& t);
    //- Construct a pure rotation septernion given a rotation quaternion
    inline explicit septernion(const quaternion& r);
    //- Construct from Istream
    septernion(Istream&);
  // Member functions
     // Access
       inline const vector& t() const;
       inline const quaternion& r() const;
     // Edit
       inline vector& t();
       inline quaternion& r();
     // Transform
       //- Transform the given vector
       inline vector transform(const vector& v) const;
       //- Inverse Transform the given vector
       inline vector invTransform(const vector& v) const;
  // Member operators
    inline void operator=(const septernion&);
    inline void operator*=(const septernion&);
    inline void operator=(const vector&);
    inline void operator+=(const vector&);
    inline void operator-=(const vector&);
    inline void operator=(const quaternion&);
    inline void operator*=(const quaternion&);
    inline void operator/=(const quaternion&);
    inline void operator*=(const scalar);
    inline void operator/=(const scalar);
  // IOstream operators
    friend Istream& operator>>(Istream& is, septernion&);
    friend Ostream& operator<<(Ostream& os, const septernion& C);
};


// Global Functions 

//- Return the inverse of the given septernion
inline septernion inv(const septernion& tr);

//- Return a string representation of a septernion
word name(const septernion&);

//- Spherical linear interpolation of septernions. 0 for qa, 1 for qb
septernion slerp
(
  const septernion& qa,
  const septernion& qb,
  const scalar t
);

//- Data associated with septernion type are contiguous
template<> inline bool contiguous<septernion>() {return true;}
// Global Operators 
inline bool operator==(const septernion& tr1, const septernion& tr2);
inline bool operator!=(const septernion& tr1, const septernion& tr2);
inline septernion operator+(const septernion& tr, const vector& t);
inline septernion operator+(const vector& t, const septernion& tr);
inline septernion operator-(const septernion& tr, const vector& t);
inline septernion operator*(const quaternion& r, const septernion& tr);
inline septernion operator*(const septernion& tr, const quaternion& r);
inline septernion operator/(const septernion& tr, const quaternion& r);
inline septernion operator*(const septernion& q1, const septernion& q2);
inline septernion operator/(const septernion& q1, const septernion& q2);
inline septernion operator*(const scalar s, const septernion& tr);
inline septernion operator*(const septernion& tr, const scalar s);
inline septernion operator/(const septernion& tr, const scalar s);
}  // namespace mousse


// Constructors 
inline mousse::septernion::septernion()
{}


inline mousse::septernion::septernion(const vector& t, const quaternion& r)
:
  t_{t},
  r_{r}
{}


inline mousse::septernion::septernion(const vector& t)
:
  t_{t},
  r_{quaternion::I}
{}


inline mousse::septernion::septernion(const quaternion& r)
:
  t_{vector::zero},
  r_{r}
{}


// Member Functions 
inline const mousse::vector& mousse::septernion::t() const
{
  return t_;
}


inline const mousse::quaternion& mousse::septernion::r() const
{
  return r_;
}


inline mousse::vector& mousse::septernion::t()
{
  return t_;
}


inline mousse::quaternion& mousse::septernion::r()
{
  return r_;
}


inline mousse::vector mousse::septernion::transform(const vector& v) const
{
  return t() + r().transform(v);
}


inline mousse::vector mousse::septernion::invTransform(const vector& v) const
{
  return r().invTransform(v - t());
}


// Member Operators 
inline void mousse::septernion::operator=(const septernion& tr)
{
  t_ = tr.t_;
  r_ = tr.r_;
}


inline void mousse::septernion::operator*=(const septernion& tr)
{
  t_ += r().transform(tr.t());
  r_ *= tr.r();
}


inline void mousse::septernion::operator=(const vector& t)
{
  t_ = t;
}


inline void mousse::septernion::operator+=(const vector& t)
{
  t_ += t;
}


inline void mousse::septernion::operator-=(const vector& t)
{
  t_ -= t;
}


inline void mousse::septernion::operator=(const quaternion& r)
{
  r_ = r;
}


inline void mousse::septernion::operator*=(const quaternion& r)
{
  r_ *= r;
}


inline void mousse::septernion::operator/=(const quaternion& r)
{
  r_ /= r;
}


inline void mousse::septernion::operator*=(const scalar s)
{
  t_ *= s;
  r_ *= s;
}


inline void mousse::septernion::operator/=(const scalar s)
{
  t_ /= s;
  r_ /= s;
}


// Global Functions 
inline mousse::septernion mousse::inv(const septernion& tr)
{
  return {-tr.r().invTransform(tr.t()), conjugate(tr.r())};
}


// Global Operators 
inline bool mousse::operator==(const septernion& tr1, const septernion& tr2)
{
  return (tr1.t() == tr2.t() && tr1.r() == tr2.r());
}


inline bool mousse::operator!=(const septernion& tr1, const septernion& tr2)
{
  return !operator==(tr1, tr2);
}


inline mousse::septernion mousse::operator+
(
  const septernion& tr,
  const vector& t
)
{
  return {tr.t() + t, tr.r()};
}


inline mousse::septernion mousse::operator+
(
  const vector& t,
  const septernion& tr
)
{
  return {t + tr.t(), tr.r()};
}


inline mousse::septernion mousse::operator-
(
  const septernion& tr,
  const vector& t
)
{
  return {tr.t() - t, tr.r()};
}


inline mousse::septernion mousse::operator*
(
  const quaternion& r,
  const septernion& tr
)
{
  return {tr.t(), r*tr.r()};
}


inline mousse::septernion mousse::operator*
(
  const septernion& tr,
  const quaternion& r
)
{
  return {tr.t(), tr.r()*r};
}


inline mousse::septernion mousse::operator/
(
  const septernion& tr,
  const quaternion& r
)
{
  return {tr.t(), tr.r()/r};
}


inline mousse::septernion mousse::operator*
(
  const septernion& tr1,
  const septernion& tr2
)
{
  return {tr1.t() + tr1.r().transform(tr2.t()),
          tr1.r().transform(tr2.r())};
}


inline mousse::septernion mousse::operator/
(
  const septernion& tr1,
  const septernion& tr2
)
{
  return tr1*inv(tr2);
}


inline mousse::septernion mousse::operator*(const scalar s, const septernion& tr)
{
  return {s*tr.t(), s*tr.r()};
}


inline mousse::septernion mousse::operator*(const septernion& tr, const scalar s)
{
  return {s*tr.t(), s*tr.r()};
}


inline mousse::septernion mousse::operator/(const septernion& tr, const scalar s)
{
  return {tr.t()/s, tr.r()/s};
}


#endif
