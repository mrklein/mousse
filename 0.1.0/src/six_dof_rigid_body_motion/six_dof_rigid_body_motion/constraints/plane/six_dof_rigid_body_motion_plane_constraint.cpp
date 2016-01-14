// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_plane_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
  DEFINE_TYPE_NAME_AND_DEBUG(plane, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    sixDoFRigidBodyMotionConstraint,
    plane,
    dictionary
  );
}
}
// Constructors 
mousse::sixDoFRigidBodyMotionConstraints::plane::plane
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
:
  sixDoFRigidBodyMotionConstraint(name, sDoFRBMCDict, motion)
{
  read(sDoFRBMCDict);
}
// Destructor 
mousse::sixDoFRigidBodyMotionConstraints::plane::~plane()
{}
// Member Functions 
void mousse::sixDoFRigidBodyMotionConstraints::plane::setCentreOfRotation
(
  point& CofR
) const
{
  CofR = centreOfRotation_;
}
void mousse::sixDoFRigidBodyMotionConstraints::plane::constrainTranslation
(
  pointConstraint& pc
) const
{
  pc.applyConstraint(normal_);
}
void mousse::sixDoFRigidBodyMotionConstraints::plane::constrainRotation
(
  pointConstraint&
) const
{}
bool mousse::sixDoFRigidBodyMotionConstraints::plane::read
(
  const dictionary& sDoFRBMCDict
)
{
  sixDoFRigidBodyMotionConstraint::read(sDoFRBMCDict);
  centreOfRotation_ = sDoFRBMCCoeffs_.lookupOrDefault
  (
    "centreOfRotation",
    motion_.initialCentreOfMass()
  );
  sDoFRBMCCoeffs_.lookup("normal") >> normal_;
  return true;
}
void mousse::sixDoFRigidBodyMotionConstraints::plane::write
(
  Ostream& os
) const
{
  os.writeKeyword("centreOfRotation")
    << centreOfRotation_ << token::END_STATEMENT << nl;
  os.writeKeyword("normal")
    << normal_ << token::END_STATEMENT << nl;
}
