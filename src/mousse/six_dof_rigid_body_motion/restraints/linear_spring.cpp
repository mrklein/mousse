// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_spring.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "six_dof_rigid_body_motion.hpp"


// Static Data Members
namespace mousse {
namespace sixDoFRigidBodyMotionRestraints {

DEFINE_TYPE_NAME_AND_DEBUG(linearSpring, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sixDoFRigidBodyMotionRestraint,
  linearSpring,
  dictionary
);

}
}


// Constructors 
mousse::sixDoFRigidBodyMotionRestraints::linearSpring::linearSpring
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
:
  sixDoFRigidBodyMotionRestraint{name, sDoFRBMRDict},
  anchor_{},
  refAttachmentPt_{},
  stiffness_{},
  damping_{},
  restLength_{}
{
  read(sDoFRBMRDict);
}


// Destructor 
mousse::sixDoFRigidBodyMotionRestraints::linearSpring::~linearSpring()
{}


// Member Functions 
void mousse::sixDoFRigidBodyMotionRestraints::linearSpring::restrain
(
  const sixDoFRigidBodyMotion& motion,
  vector& restraintPosition,
  vector& restraintForce,
  vector& restraintMoment
) const
{
  restraintPosition = motion.transform(refAttachmentPt_);
  vector r = restraintPosition - anchor_;
  scalar magR = mag(r);
  r /= (magR + VSMALL);
  vector v = motion.velocity(restraintPosition);
  restraintForce = -stiffness_*(magR - restLength_)*r - damping_*(r & v)*r;
  restraintMoment = vector::zero;
  if (motion.report()) {
    Info
      << " attachmentPt - anchor " << r*magR
      << " spring length " << magR
      << " force " << restraintForce
      << endl;
  }
}


bool mousse::sixDoFRigidBodyMotionRestraints::linearSpring::read
(
  const dictionary& sDoFRBMRDict
)
{
  sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);
  sDoFRBMRCoeffs_.lookup("anchor") >> anchor_;
  sDoFRBMRCoeffs_.lookup("refAttachmentPt") >> refAttachmentPt_;
  sDoFRBMRCoeffs_.lookup("stiffness") >> stiffness_;
  sDoFRBMRCoeffs_.lookup("damping") >> damping_;
  sDoFRBMRCoeffs_.lookup("restLength") >> restLength_;
  return true;
}


void mousse::sixDoFRigidBodyMotionRestraints::linearSpring::write
(
  Ostream& os
) const
{
  os.writeKeyword("anchor")
    << anchor_ << token::END_STATEMENT << nl;
  os.writeKeyword("refAttachmentPt")
    << refAttachmentPt_ << token::END_STATEMENT << nl;
  os.writeKeyword("stiffness")
    << stiffness_ << token::END_STATEMENT << nl;
  os.writeKeyword("damping")
    << damping_ << token::END_STATEMENT << nl;
  os.writeKeyword("restLength")
    << restLength_ << token::END_STATEMENT << nl;
}

