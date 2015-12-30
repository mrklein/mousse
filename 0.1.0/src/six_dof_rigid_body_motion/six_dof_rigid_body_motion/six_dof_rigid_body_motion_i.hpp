// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorX
(
  scalar phi
) const
{
  return tensor
  (
    1, 0, 0,
    0, mousse::cos(phi), -mousse::sin(phi),
    0, mousse::sin(phi), mousse::cos(phi)
  );
}
inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorY
(
  scalar phi
) const
{
  return tensor
  (
    mousse::cos(phi), 0, mousse::sin(phi),
    0, 1, 0,
    -mousse::sin(phi), 0, mousse::cos(phi)
  );
}
inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorZ
(
  scalar phi
) const
{
  return tensor
  (
    mousse::cos(phi), -mousse::sin(phi), 0,
    mousse::sin(phi), mousse::cos(phi), 0,
    0, 0, 1
  );
}
inline mousse::Tuple2<mousse::tensor, mousse::vector>
mousse::sixDoFRigidBodyMotion::rotate
(
  const tensor& Q0,
  const vector& pi0,
  const scalar deltaT
) const
{
  Tuple2<tensor, vector> Qpi(Q0, pi0);
  tensor& Q = Qpi.first();
  vector& pi = Qpi.second();
  tensor R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorZ(deltaT*pi.z()/momentOfInertia_.zz());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
  pi = pi & R;
  Q = Q & R;
  return Qpi;
}
inline const mousse::PtrList<mousse::sixDoFRigidBodyMotionRestraint>&
mousse::sixDoFRigidBodyMotion::restraints() const
{
  return restraints_;
}
inline const mousse::PtrList<mousse::sixDoFRigidBodyMotionConstraint>&
mousse::sixDoFRigidBodyMotion::constraints() const
{
  return constraints_;
}
inline const mousse::point&
mousse::sixDoFRigidBodyMotion::initialCentreOfRotation() const
{
  return initialCentreOfRotation_;
}
inline const mousse::tensor&
mousse::sixDoFRigidBodyMotion::initialQ() const
{
  return initialQ_;
}
inline const mousse::tensor& mousse::sixDoFRigidBodyMotion::Q() const
{
  return motionState_.Q();
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotion::v() const
{
  return motionState_.v();
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotion::a() const
{
  return motionState_.a();
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotion::pi() const
{
  return motionState_.pi();
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotion::tau() const
{
  return motionState_.tau();
}
inline mousse::point& mousse::sixDoFRigidBodyMotion::initialCentreOfRotation()
{
  return initialCentreOfRotation_;
}
inline mousse::tensor& mousse::sixDoFRigidBodyMotion::initialQ()
{
  return initialQ_;
}
inline mousse::tensor& mousse::sixDoFRigidBodyMotion::Q()
{
  return motionState_.Q();
}
inline mousse::vector& mousse::sixDoFRigidBodyMotion::v()
{
  return motionState_.v();
}
inline mousse::vector& mousse::sixDoFRigidBodyMotion::a()
{
  return motionState_.a();
}
inline mousse::vector& mousse::sixDoFRigidBodyMotion::pi()
{
  return motionState_.pi();
}
inline mousse::vector& mousse::sixDoFRigidBodyMotion::tau()
{
  return motionState_.tau();
}
// Member Functions 
inline mousse::scalar mousse::sixDoFRigidBodyMotion::mass() const
{
  return mass_;
}
inline const mousse::diagTensor&
mousse::sixDoFRigidBodyMotion::momentOfInertia() const
{
  return momentOfInertia_;
}
inline const mousse::sixDoFRigidBodyMotionState&
mousse::sixDoFRigidBodyMotion::state() const
{
  return motionState_;
}
inline const mousse::point& mousse::sixDoFRigidBodyMotion::centreOfRotation() const
{
  return motionState_.centreOfRotation();
}
inline const mousse::point&
mousse::sixDoFRigidBodyMotion::initialCentreOfMass() const
{
  return initialCentreOfMass_;
}
inline mousse::point mousse::sixDoFRigidBodyMotion::centreOfMass() const
{
  return transform(initialCentreOfMass_);
}
inline mousse::vector mousse::sixDoFRigidBodyMotion::momentArm() const
{
  return centreOfMass() - motionState_.centreOfRotation();
}
inline const mousse::tensor&
mousse::sixDoFRigidBodyMotion::orientation() const
{
  return Q();
}
inline mousse::vector mousse::sixDoFRigidBodyMotion::omega() const
{
  return  Q() & (inv(momentOfInertia_) & pi());
}
inline bool mousse::sixDoFRigidBodyMotion::report() const
{
  return report_;
}
inline void mousse::sixDoFRigidBodyMotion::newTime()
{
  motionState0_ = motionState_;
}
inline mousse::point& mousse::sixDoFRigidBodyMotion::centreOfRotation()
{
  return motionState_.centreOfRotation();
}
inline mousse::point mousse::sixDoFRigidBodyMotion::velocity
(
  const point& pt
) const
{
  return (omega() ^ (pt - centreOfRotation())) + v();
}
inline mousse::point mousse::sixDoFRigidBodyMotion::transform
(
  const point& initialPoint
) const
{
  return
  (
    centreOfRotation()
   + (Q() & initialQ_.T() & (initialPoint - initialCentreOfRotation_))
  );
}
