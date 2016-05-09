// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_point_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"


// Static Data Members
namespace mousse {
namespace sixDoFRigidBodyMotionConstraints {

DEFINE_TYPE_NAME_AND_DEBUG(point, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sixDoFRigidBodyMotionConstraint,
  point,
  dictionary
);

}
}


// Constructors 
mousse::sixDoFRigidBodyMotionConstraints::point::point
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
:
  sixDoFRigidBodyMotionConstraint{name, sDoFRBMCDict, motion},
  centreOfRotation_{}
{
  read(sDoFRBMCDict);
}


// Destructor 
mousse::sixDoFRigidBodyMotionConstraints::point::~point()
{}


// Member Functions 
void mousse::sixDoFRigidBodyMotionConstraints::point::setCentreOfRotation
(
  mousse::point& CofR
) const
{
  CofR = centreOfRotation_;
}


void mousse::sixDoFRigidBodyMotionConstraints::point::constrainTranslation
(
  pointConstraint& pc
) const
{
  pc.combine(pointConstraint(Tuple2<label, vector>(3, vector::zero)));
}


void mousse::sixDoFRigidBodyMotionConstraints::point::constrainRotation
(
  pointConstraint&
) const
{}


bool mousse::sixDoFRigidBodyMotionConstraints::point::read
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
  return true;
}


void mousse::sixDoFRigidBodyMotionConstraints::point::write
(
  Ostream& os
) const
{
  os.writeKeyword("centreOfRotation")
    << centreOfRotation_ << token::END_STATEMENT << nl;
}

