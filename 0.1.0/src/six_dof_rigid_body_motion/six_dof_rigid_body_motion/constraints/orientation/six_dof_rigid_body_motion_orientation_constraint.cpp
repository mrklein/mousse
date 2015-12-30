// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_orientation_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
  defineTypeNameAndDebug(orientation, 0);
  addToRunTimeSelectionTable
  (
    sixDoFRigidBodyMotionConstraint,
    orientation,
    dictionary
  );
}
}
// Constructors 
mousse::sixDoFRigidBodyMotionConstraints::orientation::orientation
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
mousse::sixDoFRigidBodyMotionConstraints::orientation::~orientation()
{}
// Member Functions 
void mousse::sixDoFRigidBodyMotionConstraints::orientation::
constrainTranslation
(
  pointConstraint& pc
) const
{}
void mousse::sixDoFRigidBodyMotionConstraints::orientation::
constrainRotation
(
  pointConstraint& pc
) const
{
  pc.combine(pointConstraint(Tuple2<label, vector>(3, vector::zero)));
}
bool mousse::sixDoFRigidBodyMotionConstraints::orientation::read
(
  const dictionary& sDoFRBMCDict
)
{
  sixDoFRigidBodyMotionConstraint::read(sDoFRBMCDict);
  return true;
}
void mousse::sixDoFRigidBodyMotionConstraints::orientation::write
(
  Ostream& os
) const
{
}
