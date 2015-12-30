// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_state.hpp"
// Constructors 
mousse::sixDoFRigidBodyMotionState::sixDoFRigidBodyMotionState()
:
  centreOfRotation_(vector::zero),
  Q_(I),
  v_(vector::zero),
  a_(vector::zero),
  pi_(vector::zero),
  tau_(vector::zero)
{}
mousse::sixDoFRigidBodyMotionState::sixDoFRigidBodyMotionState
(
  const dictionary& dict
)
:
  centreOfRotation_
  (
    dict.lookupOrDefault
    (
      "centreOfRotation",
      dict.lookupOrDefault("centreOfMass", vector::zero)
    )
  ),
  Q_(dict.lookupOrDefault("orientation", tensor::I)),
  v_(dict.lookupOrDefault("velocity", vector::zero)),
  a_(dict.lookupOrDefault("acceleration", vector::zero)),
  pi_(dict.lookupOrDefault("angularMomentum", vector::zero)),
  tau_(dict.lookupOrDefault("torque", vector::zero))
{}
mousse::sixDoFRigidBodyMotionState::sixDoFRigidBodyMotionState
(
  const sixDoFRigidBodyMotionState& sDoFRBMS
)
:
  centreOfRotation_(sDoFRBMS.centreOfRotation()),
  Q_(sDoFRBMS.Q()),
  v_(sDoFRBMS.v()),
  a_(sDoFRBMS.a()),
  pi_(sDoFRBMS.pi()),
  tau_(sDoFRBMS.tau())
{}
// Destructor 
mousse::sixDoFRigidBodyMotionState::~sixDoFRigidBodyMotionState()
{}
