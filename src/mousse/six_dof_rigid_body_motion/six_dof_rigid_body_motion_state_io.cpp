// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_state.hpp"
#include "iostreams.hpp"


// Member Functions 
void mousse::sixDoFRigidBodyMotionState::write(dictionary& dict) const
{
  dict.add("centreOfRotation", centreOfRotation_);
  dict.add("orientation", Q_);
  dict.add("velocity", v_);
  dict.add("acceleration", a_);
  dict.add("angularMomentum", pi_);
  dict.add("torque", tau_);
}


void mousse::sixDoFRigidBodyMotionState::write(Ostream& os) const
{
  os.writeKeyword("centreOfRotation")
    << centreOfRotation_ << token::END_STATEMENT << nl;
  os.writeKeyword("orientation")
    << Q_ << token::END_STATEMENT << nl;
  os.writeKeyword("velocity")
    << v_ << token::END_STATEMENT << nl;
  os.writeKeyword("acceleration")
    << a_ << token::END_STATEMENT << nl;
  os.writeKeyword("angularMomentum")
    << pi_ << token::END_STATEMENT << nl;
  os.writeKeyword("torque")
    << tau_ << token::END_STATEMENT << nl;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>
(
  Istream& is, sixDoFRigidBodyMotionState& sDoFRBMS
)
{
  is >> sDoFRBMS.centreOfRotation_
    >> sDoFRBMS.Q_
    >> sDoFRBMS.v_
    >> sDoFRBMS.a_
    >> sDoFRBMS.pi_
    >> sDoFRBMS.tau_;
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>"
    "(mousse::Istream&, mousse::sixDoFRigidBodyMotionState&)"
  );
  return is;
}


mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const sixDoFRigidBodyMotionState& sDoFRBMS
)
{
  os << token::SPACE << sDoFRBMS.centreOfRotation()
    << token::SPACE << sDoFRBMS.Q()
    << token::SPACE << sDoFRBMS.v()
    << token::SPACE << sDoFRBMS.a()
    << token::SPACE << sDoFRBMS.pi()
    << token::SPACE << sDoFRBMS.tau();
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::sixDoFRigidBodyMotionState&)"
  );
  return os;
}

