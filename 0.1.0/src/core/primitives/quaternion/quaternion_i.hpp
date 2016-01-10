// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
  return quaternion(-(v() & u), w()*u + (v() ^ u));
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
  return tensor
  (
    w2 + x2 - y2 - z2,  txy - twz,          txz + twy,
    txy + twz,          w2 - x2 + y2 - z2,  tyz - twx,
    txz - twy,          tyz + twx,          w2 - x2 - y2 + z2
  );
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
