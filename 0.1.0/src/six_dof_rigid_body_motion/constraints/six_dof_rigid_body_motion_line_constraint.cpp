// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_line_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"


// Static Data Members
namespace mousse {
namespace sixDoFRigidBodyMotionConstraints {

DEFINE_TYPE_NAME_AND_DEBUG(line, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sixDoFRigidBodyMotionConstraint,
  line,
  dictionary
);

}
}


// Constructors 
mousse::sixDoFRigidBodyMotionConstraints::line::line
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
:
  sixDoFRigidBodyMotionConstraint{name, sDoFRBMCDict, motion}
{
  read(sDoFRBMCDict);
}


// Destructor 
mousse::sixDoFRigidBodyMotionConstraints::line::~line()
{}


// Member Functions 
void mousse::sixDoFRigidBodyMotionConstraints::line::setCentreOfRotation
(
  point& CofR
) const
{
  CofR = centreOfRotation_;
}


void mousse::sixDoFRigidBodyMotionConstraints::line::constrainTranslation
(
  pointConstraint& pc
) const
{
  pc.combine(pointConstraint(Tuple2<label, vector>(2, direction_)));
}


void mousse::sixDoFRigidBodyMotionConstraints::line::constrainRotation
(
  pointConstraint&
) const
{}


bool mousse::sixDoFRigidBodyMotionConstraints::line::read
(
  const dictionary& sDoFRBMCDict
)
{
  sixDoFRigidBodyMotionConstraint::read(sDoFRBMCDict);
  centreOfRotation_ =
    sDoFRBMCCoeffs_.lookupOrDefault
    (
      "centreOfRotation",
      motion_.initialCentreOfMass()
    );
  sDoFRBMCCoeffs_.lookup("direction") >> direction_;
  scalar magDir{mag(direction_)};
  if (magDir > VSMALL) {
    direction_ /= magDir;
  } else {
    FATAL_ERROR_IN
    (
      "mousse::sixDoFRigidBodyMotionConstraints::line::read"
      "("
      "  const dictionary& sDoFRBMCDict"
      ")"
    )
    << "line direction has zero length"
    << abort(FatalError);
  }
  return true;
}


void mousse::sixDoFRigidBodyMotionConstraints::line::write
(
  Ostream& os
) const
{
  os.writeKeyword("centreOfRotation")
    << centreOfRotation_ << token::END_STATEMENT << nl;
  os.writeKeyword("direction")
    << direction_ << token::END_STATEMENT << nl;
}

