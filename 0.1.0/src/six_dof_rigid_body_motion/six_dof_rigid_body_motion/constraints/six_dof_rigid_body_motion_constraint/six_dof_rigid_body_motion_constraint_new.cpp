// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_constraint.hpp"
// Selectors
mousse::autoPtr<mousse::sixDoFRigidBodyMotionConstraint>
mousse::sixDoFRigidBodyMotionConstraint::New
(
  const word& name,
  const dictionary& sDoFRBMCDict,
  const sixDoFRigidBodyMotion& motion
)
{
  const word constraintType
  (
    sDoFRBMCDict.lookup("sixDoFRigidBodyMotionConstraint")
  );
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(constraintType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "sixDoFRigidBodyMotionConstraint::New"
      "("
        "const word& name,"
        "const dictionary& sDoFRBMCDict"
        "const sixDoFRigidBodyMotion& motion"
      ")"
    )   << "Unknown sixDoFRigidBodyMotionConstraint type "
      << constraintType << nl << nl
      << "Valid sixDoFRigidBodyMotionConstraints are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<sixDoFRigidBodyMotionConstraint>
  (
    cstrIter()(name, sDoFRBMCDict, motion)
  );
}
