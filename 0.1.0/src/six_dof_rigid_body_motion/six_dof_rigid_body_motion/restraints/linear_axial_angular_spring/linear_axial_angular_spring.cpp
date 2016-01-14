// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_axial_angular_spring.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
#include "transform.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
  DEFINE_TYPE_NAME_AND_DEBUG(linearAxialAngularSpring, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    sixDoFRigidBodyMotionRestraint,
    linearAxialAngularSpring,
    dictionary
  );
}
}
// Constructors 
mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::
linearAxialAngularSpring
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  sixDoFRigidBodyMotionRestraint(name, sDoFRBMRDict),
  refQ_(),
  axis_(),
  stiffness_(),
  damping_()
{
  read(sDoFRBMRDict);
}
// Destructor 
mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::
~linearAxialAngularSpring()
{}
// Member Functions 
void
mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::restrain
(
  const sixDoFRigidBodyMotion& motion,
  vector& restraintPosition,
  vector& restraintForce,
  vector& restraintMoment
) const
{
  vector refDir = rotationTensor(vector(1, 0 ,0), axis_) & vector(0, 1, 0);
  vector oldDir = refQ_ & refDir;
  vector newDir = motion.orientation() & refDir;
  if (mag(oldDir & axis_) > 0.95 || mag(newDir & axis_) > 0.95)
  {
    // Directions getting close to the axis, change reference
    refDir = rotationTensor(vector(1, 0 ,0), axis_) & vector(0, 0, 1);
    oldDir = refQ_ & refDir;
    newDir = motion.orientation() & refDir;
  }
  // Removing any axis component from oldDir and newDir and normalising
  oldDir -= (axis_ & oldDir)*axis_;
  oldDir /= (mag(oldDir) + VSMALL);
  newDir -= (axis_ & newDir)*axis_;
  newDir /= (mag(newDir) + VSMALL);
  scalar theta = mag(acos(min(oldDir & newDir, 1.0)));
  // Temporary axis with sign information.
  vector a = (oldDir ^ newDir);
  // Remove any component that is not along axis that may creep in
  a = (a & axis_)*axis_;
  scalar magA = mag(a);
  if (magA > VSMALL)
  {
    a /= magA;
  }
  else
  {
    a = vector::zero;
  }
  // Damping of along axis angular velocity only
  restraintMoment = -stiffness_*theta*a - damping_*(motion.omega() & a)*a;
  restraintForce = vector::zero;
  // Not needed to be altered as restraintForce is zero, but set to
  // centreOfRotation to be sure of no spurious moment
  restraintPosition = motion.centreOfRotation();
  if (motion.report())
  {
    Info<< " angle " << theta*sign(a & axis_)
      << " moment " << restraintMoment
      << endl;
  }
}
bool mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::read
(
  const dictionary& sDoFRBMRDict
)
{
  sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);
  refQ_ = sDoFRBMRCoeffs_.lookupOrDefault<tensor>("referenceOrientation", I);
  if (mag(mag(refQ_) - sqrt(3.0)) > 1e-9)
  {
    FATAL_ERROR_IN
    (
      "mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::"
      "read"
      "("
        "const dictionary& sDoFRBMRDict"
      ")"
    )
      << "referenceOrientation " << refQ_ << " is not a rotation tensor. "
      << "mag(referenceOrientation) - sqrt(3) = "
      << mag(refQ_) - sqrt(3.0) << nl
      << exit(FatalError);
  }
  axis_ = sDoFRBMRCoeffs_.lookup("axis");
  scalar magAxis(mag(axis_));
  if (magAxis > VSMALL)
  {
    axis_ /= magAxis;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::"
      "read"
      "("
        "const dictionary& sDoFRBMCDict"
      ")"
    )
      << "axis has zero length"
      << abort(FatalError);
  }
  sDoFRBMRCoeffs_.lookup("stiffness") >> stiffness_;
  sDoFRBMRCoeffs_.lookup("damping") >> damping_;
  return true;
}
void mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring::write
(
  Ostream& os
) const
{
  os.writeKeyword("referenceOrientation")
    << refQ_ << token::END_STATEMENT << nl;
  os.writeKeyword("axis")
    << axis_ << token::END_STATEMENT << nl;
  os.writeKeyword("stiffness")
    << stiffness_ << token::END_STATEMENT << nl;
  os.writeKeyword("damping")
    << damping_ << token::END_STATEMENT << nl;
}
