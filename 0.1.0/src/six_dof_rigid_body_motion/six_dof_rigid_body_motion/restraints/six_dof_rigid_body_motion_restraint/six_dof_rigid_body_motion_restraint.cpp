// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
#include "six_dof_rigid_body_motion_restraint.hpp"

// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(sixDoFRigidBodyMotionRestraint, 0);
  defineRunTimeSelectionTable(sixDoFRigidBodyMotionRestraint, dictionary);
}
// Constructors 
mousse::sixDoFRigidBodyMotionRestraint::sixDoFRigidBodyMotionRestraint
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  name_(name),
  sDoFRBMRCoeffs_(sDoFRBMRDict)
{}
// Destructor 
mousse::sixDoFRigidBodyMotionRestraint::~sixDoFRigidBodyMotionRestraint()
{}
// Member Functions 
bool mousse::sixDoFRigidBodyMotionRestraint::read
(
  const dictionary& sDoFRBMRDict
)
{
  sDoFRBMRCoeffs_ = sDoFRBMRDict;
  return true;
}
