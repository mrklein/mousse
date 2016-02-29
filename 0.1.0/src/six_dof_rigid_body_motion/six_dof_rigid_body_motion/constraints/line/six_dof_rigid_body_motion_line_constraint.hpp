#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_LINE_SIX_DOF_RIGID_BODY_MOTION_LINE_CONSTRAINT_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_LINE_SIX_DOF_RIGID_BODY_MOTION_LINE_CONSTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionConstraints::line
// Description
//   Translation constraint on the centre of rotation:
//     may only move along a line.
//   If 'centreOfRotation' is not provided in the dictionary the centre of mass
//   is used.
// SourceFiles
//   six_dof_rigid_body_motion_line_constraint.cpp
#include "six_dof_rigid_body_motion_constraint.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionConstraints
{
class line
:
  public sixDoFRigidBodyMotionConstraint
{
  // Private data
    //- Centre of rotation on line
    point centreOfRotation_;
    //- Direction of the constraining line
    vector direction_;
public:
  //- Runtime type information
  TYPE_NAME("line");
  // Constructors
    //- Construct from components
    line
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
        new line(*this)
      );
    }
  //- Destructor
  virtual ~line();
  // Member Functions
    //- Set the centre of rotation to the projection of the
    //  centre of mass onto the line
    virtual void setCentreOfRotation(point&) const;
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
