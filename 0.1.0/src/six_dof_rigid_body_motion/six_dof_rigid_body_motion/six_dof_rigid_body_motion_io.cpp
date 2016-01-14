// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion.hpp"
#include "iostreams.hpp"
// Member Functions 
bool mousse::sixDoFRigidBodyMotion::read(const dictionary& dict)
{
  dict.lookup("mass") >> mass_;
  dict.lookup("momentOfInertia") >> momentOfInertia_;
  aRelax_ = dict.lookupOrDefault<scalar>("accelerationRelaxation", 1.0);
  aDamp_ = dict.lookupOrDefault<scalar>("accelerationDamping", 1.0);
  report_ = dict.lookupOrDefault<Switch>("report", false);
  restraints_.clear();
  addRestraints(dict);
  constraints_.clear();
  addConstraints(dict);
  return true;
}
void mousse::sixDoFRigidBodyMotion::write(Ostream& os) const
{
  motionState_.write(os);
  os.writeKeyword("centreOfMass")
    << initialCentreOfMass_ << token::END_STATEMENT << nl;
  os.writeKeyword("initialOrientation")
    << initialQ_ << token::END_STATEMENT << nl;
  os.writeKeyword("mass")
    << mass_ << token::END_STATEMENT << nl;
  os.writeKeyword("momentOfInertia")
    << momentOfInertia_ << token::END_STATEMENT << nl;
  os.writeKeyword("accelerationRelaxation")
    << aRelax_ << token::END_STATEMENT << nl;
  os.writeKeyword("accelerationDamping")
    << aDamp_ << token::END_STATEMENT << nl;
  os.writeKeyword("report")
    << report_ << token::END_STATEMENT << nl;
  if (!restraints_.empty())
  {
    os  << indent << "restraints" << nl
      << indent << token::BEGIN_BLOCK << incrIndent << nl;
    FOR_ALL(restraints_, rI)
    {
      word restraintType = restraints_[rI].type();
      os  << indent << restraints_[rI].name() << nl
        << indent << token::BEGIN_BLOCK << incrIndent << endl;
      os.writeKeyword("sixDoFRigidBodyMotionRestraint")
        << restraintType << token::END_STATEMENT << nl;
      restraints_[rI].write(os);
      os  << decrIndent << indent << token::END_BLOCK << endl;
    }
    os  << decrIndent << indent << token::END_BLOCK << nl;
  }
  if (!constraints_.empty())
  {
    os  << indent << "constraints" << nl
      << indent << token::BEGIN_BLOCK << incrIndent << nl;
    FOR_ALL(constraints_, rI)
    {
      word constraintType = constraints_[rI].type();
      os  << indent << constraints_[rI].name() << nl
        << indent << token::BEGIN_BLOCK << incrIndent << endl;
      os.writeKeyword("sixDoFRigidBodyMotionConstraint")
        << constraintType << token::END_STATEMENT << nl;
      constraints_[rI].sixDoFRigidBodyMotionConstraint::write(os);
      constraints_[rI].write(os);
      os  << decrIndent << indent << token::END_BLOCK << endl;
    }
    os  << decrIndent << indent << token::END_BLOCK << nl;
  }
}
