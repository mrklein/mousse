// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_restraint.hpp"

// Selectors
mousse::autoPtr<mousse::sixDoFRigidBodyMotionRestraint>
mousse::sixDoFRigidBodyMotionRestraint::New
(
  const word& name,
  const dictionary& sDoFRBMRDict
)
{
  const word restraintType
  (
    sDoFRBMRDict.lookup("sixDoFRigidBodyMotionRestraint")
  );
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(restraintType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "sixDoFRigidBodyMotionRestraint::New"
      "("
        "const dictionary& sDoFRBMRDict"
      ")"
    )   << "Unknown sixDoFRigidBodyMotionRestraint type "
      << restraintType << nl << nl
      << "Valid sixDoFRigidBodyMotionRestraint types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<sixDoFRigidBodyMotionRestraint>
  (
    cstrIter()(name, sDoFRBMRDict)
  );
}
