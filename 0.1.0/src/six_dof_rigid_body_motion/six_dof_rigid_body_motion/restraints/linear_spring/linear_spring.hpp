#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_SPRING_LINEAR_SPRING_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_SPRING_LINEAR_SPRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::linearSpring
// Description
//   sixDoFRigidBodyMotionRestraints model.  Linear spring.
// SourceFiles
//   linear_spring.cpp
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "point.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class linearSpring
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Anchor point, where the spring is attached to an immovable
    //  object
    point anchor_;
    //- Reference point of attachment to the solid body
    point refAttachmentPt_;
    //- Spring stiffness coefficient (N/m)
    scalar stiffness_;
    //- Damping coefficient (Ns/m)
    scalar damping_;
    //- Rest length - length of spring when no forces are applied to it
    scalar restLength_;
public:
  //- Runtime type information
  TYPE_NAME("linearSpring");
  // Constructors
    //- Construct from components
    linearSpring
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new linearSpring(*this)
      );
    }
  //- Destructor
  virtual ~linearSpring();
  // Member Functions
    //- Calculate the restraint position, force and moment.
    //  Global reference frame vectors.
    virtual void restrain
    (
      const sixDoFRigidBodyMotion& motion,
      vector& restraintPosition,
      vector& restraintForce,
      vector& restraintMoment
    ) const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& sDoFRBMRCoeff);
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
