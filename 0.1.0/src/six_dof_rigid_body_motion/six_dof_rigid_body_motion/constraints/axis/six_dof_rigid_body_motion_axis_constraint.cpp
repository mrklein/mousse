// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_axis_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
  defineTypeNameAndDebug(axis, 0);
  addToRunTimeSelectionTable
  (
    sixDoFRigidBodyMotionConstraint,
    axis,
    dictionary
  );
}
}
// Constructors
mousse::sixDoFRigidBodyMotionConstraints::axis::axis
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
:
  sixDoFRigidBodyMotionConstraint(name, sDoFRBMCDict, motion),
  axis_()
{
  read(sDoFRBMCDict);
}
// Destructor
mousse::sixDoFRigidBodyMotionConstraints::axis::~axis()
{}
// Member Functions
void mousse::sixDoFRigidBodyMotionConstraints::axis::constrainTranslation
(
  pointConstraint& pc
) const
{}
void mousse::sixDoFRigidBodyMotionConstraints::axis::constrainRotation
(
  pointConstraint& pc
) const
{
  pc.combine(pointConstraint(Tuple2<label, vector>(2, axis_)));
}
bool mousse::sixDoFRigidBodyMotionConstraints::axis::read
(
  const dictionary& sDoFRBMCDict
)
{
  sixDoFRigidBodyMotionConstraint::read(sDoFRBMCDict);
  sDoFRBMCCoeffs_.lookup("axis") >> axis_;
  scalar magFixedAxis(mag(axis_));
  if (magFixedAxis > VSMALL)
  {
    axis_ /= magFixedAxis;
  }
  else
  {
    FatalErrorIn
    (
      "mousse::sixDoFRigidBodyMotionConstraints::axis::read"
      "("
        "const dictionary& sDoFRBMCDict"
      ")"
    )
      << "axis has zero length"
      << abort(FatalError);
  }
  return true;
}
void mousse::sixDoFRigidBodyMotionConstraints::axis::write
(
  Ostream& os
) const
{
  os.writeKeyword("axis")
    << axis_ << token::END_STATEMENT << nl;
}
