#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_ORIENTATION_SIX_DOF_RIGID_BODY_MOTION_ORIENTATION_CONSTRAINT_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_ORIENTATION_SIX_DOF_RIGID_BODY_MOTION_ORIENTATION_CONSTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionConstraints::orientation
// Description
//   Orientation constraint:
//     fixed in global space.
// SourceFiles
//   six_dof_rigid_body_motion_orientation_constraint.cpp
#include "six_dof_rigid_body_motion_constraint.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
class orientation
:
  public sixDoFRigidBodyMotionConstraint
{
public:
  //- Runtime type information
  TYPE_NAME("orientation");
  // Constructors
    //- Construct from components
    orientation
    (
      const word& name,
      const dictionary& sDoFRBMCDict,
      const sixDoFRigidBodyMotion& motion
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionConstraint>
      (
        new orientation(*this)
      );
    }
  //- Destructor
  virtual ~orientation();
  // Member Functions
    //- Apply and accumulate translational constraints
    virtual void constrainTranslation(pointConstraint&) const;
    //- Apply and accumulate rotational constraints
    virtual void constrainRotation(pointConstraint&) const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& sDoFRBMCCoeff);
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
