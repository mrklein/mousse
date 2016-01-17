// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::quaternion
// Description
//   Quaternion class used to perform rotations in 3D space.
// SourceFiles
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

// Constructors 
inline mousse::quaternion::quaternion()
{}

inline mousse::quaternion::quaternion(const scalar w, const vector& v)
:
  w_{w},
  v_{v}
{}

inline mousse::quaternion::quaternion(const vector& d, const scalar theta)
:
  w_{cos(0.5*theta)},
  v_{(sin(0.5*theta)/mag(d))*d}
{}

inline mousse::quaternion::quaternion
(
  const vector& d,
  const scalar cosTheta,
  const bool normalized
)
{
  scalar cosHalfTheta2 = 0.5*(cosTheta + 1);
  w_ = sqrt(cosHalfTheta2);
  if (normalized)
  {
    v_ = sqrt(1 - cosHalfTheta2)*d;
  }
  else
  {
    v_ = (sqrt(1 - cosHalfTheta2)/mag(d))*d;
  }
}


inline mousse::quaternion::quaternion(const scalar w)
:
  w_{w},
  v_{vector::zero}
{}


inline mousse::quaternion::quaternion(const vector& v)
:
  w_{0},
  v_{v}
{}


inline mousse::quaternion::quaternion
(
  const scalar angleX,
  const scalar angleY,
  const scalar angleZ
)
{
  operator=(quaternion(vector(1, 0, 0), angleX));
  operator*=(quaternion(vector(0, 1, 0), angleY));
  operator*=(quaternion(vector(0, 0, 1), angleZ));
}


inline mousse::quaternion::quaternion
(
  const tensor& rotationTensor
)
{
  scalar trace =
    rotationTensor.xx()
   + rotationTensor.yy()
   + rotationTensor.zz();
  if (trace > 0)
  {
    scalar s = 0.5/mousse::sqrt(trace + 1.0);
    w_ = 0.25/s;
    v_[0] = (rotationTensor.zy() - rotationTensor.yz())*s;
    v_[1] = (rotationTensor.xz() - rotationTensor.zx())*s;
    v_[2] = (rotationTensor.yx() - rotationTensor.xy())*s;
  }
  else
  {
    if
    (
      rotationTensor.xx() > rotationTensor.yy()
      && rotationTensor.xx() > rotationTensor.zz()
    )
    {
      scalar s = 2.0*mousse::sqrt
      (
        1.0
       + rotationTensor.xx()
       - rotationTensor.yy()
       - rotationTensor.zz()
      );
      w_ = (rotationTensor.zy() - rotationTensor.yz())/s;
      v_[0] = 0.25*s;
      v_[1] = (rotationTensor.xy() + rotationTensor.yx())/s;
      v_[2] = (rotationTensor.xz() + rotationTensor.zx())/s;
    }
    else if
    (
      rotationTensor.yy() > rotationTensor.zz()
    )
    {
      scalar s = 2.0*mousse::sqrt
      (
        1.0
       + rotationTensor.yy()
       - rotationTensor.xx()
       - rotationTensor.zz()
      );
      w_ = (rotationTensor.xz() - rotationTensor.xz())/s;
      v_[0] = (rotationTensor.xy() + rotationTensor.yx())/s;
      v_[1] = 0.25*s;
      v_[2] = (rotationTensor.yz() + rotationTensor.zy())/s;
    }
    else
    {
      scalar s = 2.0*mousse::sqrt
      (
        1.0
       + rotationTensor.zz()
       - rotationTensor.xx()
       - rotationTensor.yy()
      );
      w_ = (rotationTensor.yx() - rotationTensor.xy())/s;
      v_[0] = (rotationTensor.xz() + rotationTensor.zx())/s;
      v_[1] = (rotationTensor.yz() + rotationTensor.zy())/s;
      v_[2] = 0.25*s;
    }
  }
}


// Member Functions 
inline mousse::scalar mousse::quaternion::w() const
{
  return w_;
}


inline const mousse::vector& mousse::quaternion::v() const
{
  return v_;
}


inline mousse::scalar& mousse::quaternion::w()
{
  return w_;
}


inline mousse::vector& mousse::quaternion::v()
{
  return v_;
}


inline mousse::quaternion mousse::quaternion::normalized() const
{
  return *this/mag(*this);
}


inline void mousse::quaternion::normalize()
{
  operator/=(mag(*this));
}


inline mousse::quaternion mousse::quaternion::mulq0v(const vector& u) const
{
  return {-(v() & u), w()*u + (v() ^ u)};
}


inline mousse::vector mousse::quaternion::transform(const vector& u) const
{
  return (mulq0v(u)*conjugate(*this)).v();
}


inline mousse::vector mousse::quaternion::invTransform(const vector& u) const
{
  return (conjugate(*this).mulq0v(u)*(*this)).v();
}


inline mousse::quaternion mousse::quaternion::transform(const quaternion& q) const
{
  return mousse::normalize((*this)*q);
}


inline mousse::quaternion mousse::quaternion::invTransform
(
  const quaternion& q
) const
{
  return mousse::normalize(conjugate(*this)*q);
}


inline mousse::tensor mousse::quaternion::R() const
{
  scalar w2 = sqr(w());
  scalar x2 = sqr(v().x());
  scalar y2 = sqr(v().y());
  scalar z2 = sqr(v().z());
  scalar txy = 2*v().x()*v().y();
  scalar twz = 2*w()*v().z();
  scalar txz = 2*v().x()*v().z();
  scalar twy = 2*w()*v().y();
  scalar tyz = 2*v().y()*v().z();
  scalar twx = 2*w()*v().x();
  return {w2 + x2 - y2 - z2,  txy - twz,          txz + twy,
          txy + twz,          w2 - x2 + y2 - z2,  tyz - twx,
          txz - twy,          tyz + twx,          w2 - x2 - y2 + z2};
}


inline mousse::vector mousse::quaternion::eulerAngles(const quaternion& q) const
{
  vector angles(vector::zero);
  const scalar& w = q.w();
  const vector& v = q.v();
  angles[0] = mousse::atan2
  (
    2*(w*v.x() + v.y()*v.z()),
    1 - 2*(sqr(v.x()) + sqr(v.y()))
  );
  angles[1] = mousse::asin(2*(w*v.y() - v.z()*v.x()));
  angles[2] = mousse::atan2
  (
    2*(w*v.z() + v.x()*v.y()),
    1 - 2*(sqr(v.y()) + sqr(v.z()))
  );

  return angles;
}


// Member Operators 
inline void mousse::quaternion::operator=(const quaternion& q)
{
  w_ = q.w_;
  v_ = q.v_;
}


inline void mousse::quaternion::operator+=(const quaternion& q)
{
  w_ += q.w_;
  v_ += q.v_;
}


inline void mousse::quaternion::operator-=(const quaternion& q)
{
  w_ -= q.w_;
  v_ -= q.v_;
}


inline void mousse::quaternion::operator*=(const quaternion& q)
{
  scalar w0 = w();
  w() = w()*q.w() - (v() & q.v());
  v() = w0*q.v() + q.w()*v() + (v() ^ q.v());
}


inline void mousse::quaternion::operator/=(const quaternion& q)
{
  return operator*=(inv(q));
}


inline void mousse::quaternion::operator=(const scalar s)
{
  w_ = s;
}


inline void mousse::quaternion::operator=(const vector& v)
{
  v_ = v;
}


inline void mousse::quaternion::operator*=(const scalar s)
{
  w_ *= s;
  v_ *= s;
}


inline void mousse::quaternion::operator/=(const scalar s)
{
  w_ /= s;
  v_ /= s;
}


// Global Functions 
inline mousse::scalar mousse::magSqr(const quaternion& q)
{
  return magSqr(q.w()) + magSqr(q.v());
}


inline mousse::scalar mousse::mag(const quaternion& q)
{
  return sqrt(magSqr(q));
}


inline mousse::quaternion mousse::conjugate(const quaternion& q)
{
  return quaternion(q.w(), -q.v());
}


inline mousse::quaternion mousse::inv(const quaternion& q)
{
  scalar magSqrq = magSqr(q);
  return quaternion(q.w()/magSqrq, -q.v()/magSqrq);
}


inline mousse::quaternion mousse::normalize(const quaternion& q)
{
  return q/mag(q);
}


// Global Operators 
inline bool mousse::operator==(const quaternion& q1, const quaternion& q2)
{
  return (equal(q1.w(), q2.w()) && equal(q1.v(), q2.v()));
}


inline bool mousse::operator!=(const quaternion& q1, const quaternion& q2)
{
  return !operator==(q1, q2);
}


inline mousse::quaternion mousse::operator+
(
  const quaternion& q1,
  const quaternion& q2
)
{
  return quaternion(q1.w() + q2.w(), q1.v() + q2.v());
}


inline mousse::quaternion mousse::operator-(const quaternion& q)
{
  return quaternion(-q.w(), -q.v());
}


inline mousse::quaternion mousse::operator-
(
  const quaternion& q1,
  const quaternion& q2
)
{
  return quaternion(q1.w() - q2.w(), q1.v() - q2.v());
}


inline mousse::scalar mousse::operator&(const quaternion& q1, const quaternion& q2)
{
  return q1.w()*q2.w() + (q1.v() & q2.v());
}


inline mousse::quaternion mousse::operator*
(
  const quaternion& q1,
  const quaternion& q2
)
{
  return quaternion
  {
    q1.w()*q2.w() - (q1.v() & q2.v()),
    q1.w()*q2.v() + q2.w()*q1.v() + (q1.v() ^ q2.v())
  };
}


inline mousse::quaternion mousse::operator/
(
  const quaternion& q1,
  const quaternion& q2
)
{
  return q1*inv(q2);
}


inline mousse::quaternion mousse::operator*(const scalar s, const quaternion& q)
{
  return quaternion(s*q.w(), s*q.v());
}


inline mousse::quaternion mousse::operator*(const quaternion& q, const scalar s)
{
  return quaternion(s*q.w(), s*q.v());
}


inline mousse::quaternion mousse::operator/(const quaternion& q, const scalar s)
{
  return quaternion(q.w()/s, q.v()/s);
}
#endif
