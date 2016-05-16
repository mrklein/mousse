// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_angular_spring.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"


// Static Data Members
namespace mousse {
namespace sixDoFRigidBodyMotionRestraints {

DEFINE_TYPE_NAME_AND_DEBUG(sphericalAngularSpring, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sixDoFRigidBodyMotionRestraint,
  sphericalAngularSpring,
  dictionary
);

}
}


// Constructors
mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::
sphericalAngularSpring
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  sixDoFRigidBodyMotionRestraint{name, sDoFRBMRDict},
  refQ_{},
  stiffness_{},
  damping_{}
{
  read(sDoFRBMRDict);
}


// Destructor
mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::
~sphericalAngularSpring()
{}


// Member Functions
void mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::restrain
(
  const sixDoFRigidBodyMotion& motion,
  vector& restraintPosition,
  vector& restraintForce,
  vector& restraintMoment
) const
{
  restraintMoment = vector::zero;
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
    vector axis = vector::zero;
    axis[cmpt] = 1;
    vector refDir = vector::zero;
    refDir[(cmpt + 1) % 3] = 1;
    vector newDir = motion.orientation() & refDir;
    axis = (refQ_ & axis);
    refDir = (refQ_ & refDir);
    newDir -= (axis & newDir)*axis;
    restraintMoment += -stiffness_*(refDir ^ newDir);
  }
  restraintMoment += -damping_*motion.omega();
  restraintForce = vector::zero;
  // Not needed to be altered as restraintForce is zero, but set to
  // centreOfRotation to be sure of no spurious moment
  restraintPosition = motion.centreOfRotation();
  if (motion.report()) {
    Info << " moment " << restraintMoment << endl;
  }
}


bool mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::read
(
  const dictionary& sDoFRBMRDict
)
{
  sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);
  refQ_ = sDoFRBMRCoeffs_.lookupOrDefault<tensor>("referenceOrientation", I);
  if (mag(mag(refQ_) - sqrt(3.0)) > 1e-9) {
    FATAL_ERROR_IN
    (
      "mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::"
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
  sDoFRBMRCoeffs_.lookup("stiffness") >> stiffness_;
  sDoFRBMRCoeffs_.lookup("damping") >> damping_;
  return true;
}


void mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::write
(
  Ostream& os
) const
{
  os.writeKeyword("referenceOrientation")
    << refQ_ << token::END_STATEMENT << nl;
  os.writeKeyword("stiffness") << stiffness_ << token::END_STATEMENT << nl;
  os.writeKeyword("damping") << damping_ << token::END_STATEMENT << nl;
}

