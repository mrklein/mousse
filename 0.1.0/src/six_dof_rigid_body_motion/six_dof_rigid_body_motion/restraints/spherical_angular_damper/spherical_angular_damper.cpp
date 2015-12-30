// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_angular_damper.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
  defineTypeNameAndDebug(sphericalAngularDamper, 0);
  addToRunTimeSelectionTable
  (
    sixDoFRigidBodyMotionRestraint,
    sphericalAngularDamper,
    dictionary
  );
}
}
// Constructors 
mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper::
sphericalAngularDamper
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  sixDoFRigidBodyMotionRestraint(name, sDoFRBMRDict),
  coeff_()
{
  read(sDoFRBMRDict);
}
// Destructor 
mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper::
~sphericalAngularDamper()
{}
// Member Functions 
void mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper::restrain
(
  const sixDoFRigidBodyMotion& motion,
  vector& restraintPosition,
  vector& restraintForce,
  vector& restraintMoment
) const
{
  restraintMoment = -coeff_*motion.omega();
  restraintForce = vector::zero;
  if (motion.report())
  {
    Info<< " moment " << restraintMoment
      << endl;
  }
}
bool mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper::read
(
  const dictionary& sDoFRBMRDict
)
{
  sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);
  sDoFRBMRCoeffs_.lookup("coeff") >> coeff_;
  return true;
}
void mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper::write
(
  Ostream& os
) const
{
  os.writeKeyword("coeff") << coeff_ << token::END_STATEMENT << nl;
}
