#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SPHERICAL_ANGULAR_DAMPER_SPHERICAL_ANGULAR_DAMPER_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SPHERICAL_ANGULAR_DAMPER_SPHERICAL_ANGULAR_DAMPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper
// Description
//   sixDoFRigidBodyMotionRestraints model.  Spherical angular damper.

#include "six_dof_rigid_body_motion_restraint.hpp"


namespace mousse {
namespace sixDoFRigidBodyMotionRestraints {

class sphericalAngularDamper
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Damping coefficient (Nms/rad)
    scalar coeff_;
public:
  //- Runtime type information
  TYPE_NAME("sphericalAngularDamper");
  // Constructors
    //- Construct from components
    sphericalAngularDamper
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return
        autoPtr<sixDoFRigidBodyMotionRestraint>
        {
          new sphericalAngularDamper{*this}
        };
    }
  //- Destructor
  virtual ~sphericalAngularDamper();
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

