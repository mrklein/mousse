#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SPHERICAL_ANGULAR_SPRING_SPHERICAL_ANGULAR_SPRING_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SPHERICAL_ANGULAR_SPRING_SPHERICAL_ANGULAR_SPRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring
// Description
//   sixDoFRigidBodyMotionRestraints model.  Spherical angular spring.
// SourceFiles
//   spherical_angular_spring.cpp
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class sphericalAngularSpring
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Reference orientation where there is no moment
    tensor refQ_;
    //- Spring stiffness coefficient (Nm/rad)
    scalar stiffness_;
    //- Damping coefficient (Nms/rad)
    scalar damping_;
public:
  //- Runtime type information
  TYPE_NAME("sphericalAngularSpring");
  // Constructors
    //- Construct from components
    sphericalAngularSpring
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new sphericalAngularSpring(*this)
      );
    }
  //- Destructor
  virtual ~sphericalAngularSpring();
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
