// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::point&
mousse::sixDoFRigidBodyMotionState::centreOfRotation() const
{
  return centreOfRotation_;
}
inline const mousse::tensor& mousse::sixDoFRigidBodyMotionState::Q() const
{
  return Q_;
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::v() const
{
  return v_;
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::a() const
{
  return a_;
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::pi() const
{
  return pi_;
}
inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::tau() const
{
  return tau_;
}
inline mousse::point& mousse::sixDoFRigidBodyMotionState::centreOfRotation()
{
  return centreOfRotation_;
}
inline mousse::tensor& mousse::sixDoFRigidBodyMotionState::Q()
{
  return Q_;
}
inline mousse::vector& mousse::sixDoFRigidBodyMotionState::v()
{
  return v_;
}
inline mousse::vector& mousse::sixDoFRigidBodyMotionState::a()
{
  return a_;
}
inline mousse::vector& mousse::sixDoFRigidBodyMotionState::pi()
{
  return pi_;
}
inline mousse::vector& mousse::sixDoFRigidBodyMotionState::tau()
{
  return tau_;
}
