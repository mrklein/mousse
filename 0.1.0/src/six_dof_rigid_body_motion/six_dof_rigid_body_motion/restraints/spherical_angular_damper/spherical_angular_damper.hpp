// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::sphericalAngularDamper
// Description
//   sixDoFRigidBodyMotionRestraints model.  Spherical angular damper.
// SourceFiles
//   spherical_angular_damper.cpp
#ifndef spherical_angular_damper_hpp_
#define spherical_angular_damper_hpp_
#include "six_dof_rigid_body_motion_restraint.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class sphericalAngularDamper
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Damping coefficient (Nms/rad)
    scalar coeff_;
public:
  //- Runtime type information
  TypeName("sphericalAngularDamper");
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
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new sphericalAngularDamper(*this)
      );
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
