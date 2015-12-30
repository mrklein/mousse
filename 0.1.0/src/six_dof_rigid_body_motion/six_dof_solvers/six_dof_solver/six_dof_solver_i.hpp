// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Protected Member Functions 
inline mousse::point& mousse::sixDoFSolver::centreOfRotation()
{
  return body_.motionState_.centreOfRotation();
}
inline mousse::tensor& mousse::sixDoFSolver::Q()
{
  return body_.motionState_.Q();
}
inline mousse::vector& mousse::sixDoFSolver::v()
{
  return body_.motionState_.v();
}
inline mousse::vector& mousse::sixDoFSolver::a()
{
  return body_.motionState_.a();
}
inline mousse::vector& mousse::sixDoFSolver::pi()
{
  return body_.motionState_.pi();
}
inline mousse::vector& mousse::sixDoFSolver::tau()
{
  return body_.motionState_.tau();
}
inline const mousse::point& mousse::sixDoFSolver::centreOfRotation0() const
{
  return body_.motionState0_.centreOfRotation();
}
inline const mousse::tensor& mousse::sixDoFSolver::Q0() const
{
  return body_.motionState0_.Q();
}
inline const mousse::vector& mousse::sixDoFSolver::v0() const
{
  return body_.motionState0_.v();
}
inline const mousse::vector& mousse::sixDoFSolver::a0() const
{
  return body_.motionState0_.a();
}
inline const mousse::vector& mousse::sixDoFSolver::pi0() const
{
  return body_.motionState0_.pi();
}
inline const mousse::vector& mousse::sixDoFSolver::tau0() const
{
  return body_.motionState0_.tau();
}
inline mousse::scalar mousse::sixDoFSolver::aDamp() const
{
  return body_.aDamp_;
}
inline mousse::tensor mousse::sixDoFSolver::tConstraints() const
{
  return body_.tConstraints_;
}
inline mousse::tensor mousse::sixDoFSolver::rConstraints() const
{
  return body_.rConstraints_;
}
//- Apply rotation tensors to Q0 for the given torque (pi) and deltaT
//  and return the rotated Q and pi as a tuple
inline mousse::Tuple2<mousse::tensor, mousse::vector> mousse::sixDoFSolver::rotate
(
  const tensor& Q0,
  const vector& pi,
  const scalar deltaT
) const
{
  return body_.rotate(Q0, pi, deltaT);
}
//- Update and relax accelerations from the force and torque
inline void mousse::sixDoFSolver::updateAcceleration
(
  const vector& fGlobal,
  const vector& tauGlobal
)
{
  body_.updateAcceleration(fGlobal, tauGlobal);
}
