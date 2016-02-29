#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_AXIAL_ANGULAR_SPRING_LINEAR_AXIAL_ANGULAR_SPRING_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_LINEAR_AXIAL_ANGULAR_SPRING_LINEAR_AXIAL_ANGULAR_SPRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::linearAxialAngularSpring
// Description
//   sixDoFRigidBodyMotionRestraints model.  Linear axial angular spring.
// SourceFiles
//   linear_axial_angular_spring.cpp
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class linearAxialAngularSpring
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Reference orientation where there is no moment
    tensor refQ_;
    //- Global unit axis around which the motion is sprung
    vector axis_;
    //- Spring stiffness coefficient (Nm/rad)
    scalar stiffness_;
    //- Damping coefficient (Nms/rad)
    scalar damping_;
public:
  //- Runtime type information
  TYPE_NAME("linearAxialAngularSpring");
  // Constructors
    //- Construct from components
    linearAxialAngularSpring
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new linearAxialAngularSpring(*this)
      );
    }
  //- Destructor
  virtual ~linearAxialAngularSpring();
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
