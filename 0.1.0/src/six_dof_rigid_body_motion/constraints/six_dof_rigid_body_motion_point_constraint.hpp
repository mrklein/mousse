#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_POINT_SIX_DOF_RIGID_BODY_MOTION_POINT_CONSTRAINT_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_POINT_SIX_DOF_RIGID_BODY_MOTION_POINT_CONSTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionConstraints::point
// Description
//   Translation constraint on the centre of rotation:
//     fixed in space.
//   If 'centreOfRotation' is not provided in the dictionary the centre of mass
//   is used.

#include "six_dof_rigid_body_motion_constraint.hpp"
#include "point.hpp"


namespace mousse {
namespace sixDoFRigidBodyMotionConstraints {

class point
:
  public sixDoFRigidBodyMotionConstraint
{
  // Private data
    //- Fixed centre of rotation
    mousse::point centreOfRotation_;
public:
  //- Runtime type information
  TYPE_NAME("point");
  // Constructors
    //- Construct from components
    point
    (
      const word& name,
      const dictionary& sDoFRBMCDict,
      const sixDoFRigidBodyMotion& motion
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const
    {
      return
        autoPtr<sixDoFRigidBodyMotionConstraint>
        {
          new point{*this}
        };
    }
  //- Destructor
  virtual ~point();
  // Member Functions
    //- Set the centre of rotation to the point
    virtual void setCentreOfRotation(mousse::point&) const;
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

