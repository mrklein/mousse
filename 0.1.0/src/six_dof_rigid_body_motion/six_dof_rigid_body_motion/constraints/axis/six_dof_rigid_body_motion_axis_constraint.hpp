// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionConstraints::axis
// Description
//   Orientation constraint:
//     may only rotate around a fixed axis.
// SourceFiles
//   six_dof_rigid_body_motion_axis_constraint.cpp
#ifndef six_dof_rigid_body_motion_axis_constraint_hpp_
#define six_dof_rigid_body_motion_axis_constraint_hpp_
#include "six_dof_rigid_body_motion_constraint.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
class axis
:
  public sixDoFRigidBodyMotionConstraint
{
  // Private data
    //- Reference axis in global space
    vector axis_;
public:
  //- Runtime type information
  TYPE_NAME("axis");
  // Constructors
    //- Construct from components
    axis
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
        new axis(*this)
      );
    }
  //- Destructor
  virtual ~axis();
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
