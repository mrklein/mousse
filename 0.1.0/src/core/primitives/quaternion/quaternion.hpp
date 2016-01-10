// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::quaternion
// Description
//   Quaternion class used to perform rotations in 3D space.
// SourceFiles
//   quaternion_i.hpp
//   quaternion.cpp
#ifndef quaternion_hpp_
#define quaternion_hpp_
#include "scalar.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "word.hpp"
#include "contiguous.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class quaternion;
Istream& operator>>(Istream& is, quaternion&);
Ostream& operator<<(Ostream& os, const quaternion& C);
class quaternion
{
  // private data
    //- Scalar part of the quaternion ( = cos(theta/2) for rotation)
    scalar w_;
    //- Vector part of the quaternion ( = axis of rotation)
    vector v_;
    //- Multiply vector v by quaternion as if v is a pure quaternion
    inline quaternion mulq0v(const vector& v) const;
public:
  //- Component type
  typedef scalar cmptType;
  // Member constants
    enum
    {
      rank = 1 // Rank of quaternion is 1
    };
  // Static data members
    static const char* const typeName;
    static const quaternion zero;
    static const quaternion I;
  // Constructors
    //- Construct null
    inline quaternion();
    //- Construct given scalar and vector parts
    inline quaternion(const scalar w, const vector& v);
    //- Construct a rotation quaternion given the direction d
    //  and angle theta
    inline quaternion(const vector& d, const scalar theta);
    //- Construct a rotation quaternion given the direction d
    //  and cosine angle cosTheta and a if d is normalized
    inline quaternion
    (
      const vector& d,
      const scalar cosTheta,
      const bool normalized
    );
    //- Construct given scalar part, the vector part = vector::zero
    inline explicit quaternion(const scalar w);
    //- Construct a pure quaternion given the vector part, scalar part = 0
    inline explicit quaternion(const vector& v);
    //- Construct a quaternion given the three Euler angles
    inline quaternion
    (
      const scalar angleX,
      const scalar angleY,
      const scalar angleZ
    );
    //- Construct a quaternion from a rotation tensor
    inline explicit quaternion(const tensor& rotationTensor);
    //- Construct from Istream
    quaternion(Istream&);
  // Member functions
     // Access
       //- Scalar part of the quaternion ( = cos(theta/2) for rotation)
       inline scalar w() const;
       //- Vector part of the quaternion ( = axis of rotation)
       inline const vector& v() const;
       //- The rotation tensor corresponding the quaternion
       inline tensor R() const;
       //- Return a vector of euler angles (rotations in radians about
       //  the x, y and z axes.
       inline vector eulerAngles(const quaternion& q) const;
       inline quaternion normalized() const;
     // Edit
       //- Scalar part of the quaternion ( = cos(theta/2) for rotation)
       inline scalar& w();
       //- Vector part of the quaternion ( = axis of rotation)
       inline vector& v();
       inline void normalize();
     // Transform
       //- Rotate the given vector
       inline vector transform(const vector& v) const;
       //- Rotate the given vector anti-clockwise
       inline vector invTransform(const vector& v) const;
       //- Rotate the given quaternion (and normalize)
       inline quaternion transform(const quaternion& q) const;
       //- Rotate the given quaternion anti-clockwise (and normalize)
       inline quaternion invTransform(const quaternion& q) const;
  // Member operators
    inline void operator=(const quaternion&);
    inline void operator+=(const quaternion&);
    inline void operator-=(const quaternion&);
    inline void operator*=(const quaternion&);
    inline void operator/=(const quaternion&);
    inline void operator=(const scalar);
    inline void operator=(const vector&);
    inline void operator*=(const scalar);
    inline void operator/=(const scalar);
  // IOstream operators
    friend Istream& operator>>(Istream& is, quaternion&);
    friend Ostream& operator<<(Ostream& os, const quaternion& C);
};
// Global Functions 
inline scalar magSqr(const quaternion& q);
inline scalar mag(const quaternion& q);
//- Return the conjugate of the given quaternion
inline quaternion conjugate(const quaternion& q);
//- Return the normalized (unit) quaternion of the given quaternion
inline quaternion normalize(const quaternion& q);
//- Return the inverse of the given quaternion
inline quaternion inv(const quaternion& q);
//- Return a string representation of a quaternion
word name(const quaternion&);
//- Spherical linear interpolation of quaternions
quaternion slerp
(
  const quaternion& qa,
  const quaternion& qb,
  const scalar t
);
//- Exponent of a quaternion
quaternion exp(const quaternion& q);
//- Power of a quaternion
quaternion pow(const quaternion& q, const label power);
//- Power of a quaternion
quaternion pow(const quaternion& q, const scalar power);
//- Data associated with quaternion type are contiguous
template<>
inline bool contiguous<quaternion>() {return true;}
// Global Operators 
inline bool operator==(const quaternion& q1, const quaternion& q2);
inline bool operator!=(const quaternion& q1, const quaternion& q2);
inline quaternion operator+(const quaternion& q1, const quaternion& q2);
inline quaternion operator-(const quaternion& q);
inline quaternion operator-(const quaternion& q1, const quaternion& q2);
inline scalar operator&(const quaternion& q1, const quaternion& q2);
inline quaternion operator*(const quaternion& q1, const quaternion& q2);
inline quaternion operator/(const quaternion& q1, const quaternion& q2);
inline quaternion operator*(const scalar s, const quaternion& q);
inline quaternion operator*(const quaternion& q, const scalar s);
inline quaternion operator/(const quaternion& q, const scalar s);
}  // namespace mousse
#include "quaternion_i.hpp"
#endif
