// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_damper.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
DEFINE_TYPE_NAME_AND_DEBUG(linearDamper, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sixDoFRigidBodyMotionRestraint,
  linearDamper,
  dictionary
);
}
}
// Constructors 
mousse::sixDoFRigidBodyMotionRestraints::linearDamper::linearDamper
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  sixDoFRigidBodyMotionRestraint{name, sDoFRBMRDict},
  coeff_{}
{
  read(sDoFRBMRDict);
}
// Destructor 
mousse::sixDoFRigidBodyMotionRestraints::linearDamper::~linearDamper()
{}
// Member Functions 
void mousse::sixDoFRigidBodyMotionRestraints::linearDamper::restrain
(
  const sixDoFRigidBodyMotion& motion,
  vector& /*restraintPosition*/,
  vector& restraintForce,
  vector& restraintMoment
) const
{
  restraintForce = -coeff_*motion.v();
  restraintMoment = vector::zero;
  if (motion.report())
  {
    Info<< " force " << restraintForce
      << endl;
  }
}
bool mousse::sixDoFRigidBodyMotionRestraints::linearDamper::read
(
  const dictionary& sDoFRBMRDict
)
{
  sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);
  sDoFRBMRCoeffs_.lookup("coeff") >> coeff_;
  return true;
}
void mousse::sixDoFRigidBodyMotionRestraints::linearDamper::write
(
  Ostream& os
) const
{
  os.writeKeyword("coeff")
    << coeff_ << token::END_STATEMENT << nl;
}
