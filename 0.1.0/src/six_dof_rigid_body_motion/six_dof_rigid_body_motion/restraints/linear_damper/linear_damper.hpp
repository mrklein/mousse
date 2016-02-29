#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_DAMPER_LINEAR_DAMPER_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_DAMPER_LINEAR_DAMPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::linearDamper
// Description
//   sixDoFRigidBodyMotionRestraints model.  Linear spring.
// SourceFiles
//   linear_damper.cpp
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "point.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class linearDamper
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Damping coefficient (Ns/m)
    scalar coeff_;
public:
  //- Runtime type information
  TYPE_NAME("linearDamper");
  // Constructors
    //- Construct from components
    linearDamper
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new linearDamper(*this)
      );
    }
  //- Destructor
  virtual ~linearDamper();
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
