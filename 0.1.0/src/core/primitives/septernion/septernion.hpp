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
//   septernion_i.hpp
//   septernion.cpp
#ifndef septernion_hpp_
#define septernion_hpp_
#include "vector.hpp"
#include "quaternion.hpp"
#include "word.hpp"
#include "contiguous.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class septernion;
Istream& operator>>(Istream& is, septernion&);
Ostream& operator<<(Ostream& os, const septernion& C);
class septernion
{
  // private data
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
template<>
inline bool contiguous<septernion>() {return true;}
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
#include "septernion_i.hpp"
#endif
