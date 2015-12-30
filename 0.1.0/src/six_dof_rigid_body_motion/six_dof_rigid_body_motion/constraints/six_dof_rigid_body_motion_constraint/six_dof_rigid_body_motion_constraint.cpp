// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_constraint.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(sixDoFRigidBodyMotionConstraint, 0);
  defineRunTimeSelectionTable(sixDoFRigidBodyMotionConstraint, dictionary);
}
// Constructors
mousse::sixDoFRigidBodyMotionConstraint::sixDoFRigidBodyMotionConstraint
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
:
  name_(name),
  sDoFRBMCCoeffs_(sDoFRBMCDict),
  motion_(motion)
{}
// Destructor
mousse::sixDoFRigidBodyMotionConstraint::~sixDoFRigidBodyMotionConstraint()
{}
// Member Functions
bool mousse::sixDoFRigidBodyMotionConstraint::read
(
  const dictionary& sDoFRBMCDict
)
{
  sDoFRBMCCoeffs_ = sDoFRBMCDict;
  return true;
}
void mousse::sixDoFRigidBodyMotionConstraint::write(Ostream& os) const
{}
