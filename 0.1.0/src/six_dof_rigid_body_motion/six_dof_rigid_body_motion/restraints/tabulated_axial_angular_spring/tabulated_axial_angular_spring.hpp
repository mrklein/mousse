#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_TABULATED_AXIAL_ANGULAR_SPRING_TABULATED_AXIAL_ANGULAR_SPRING_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_TABULATED_AXIAL_ANGULAR_SPRING_TABULATED_AXIAL_ANGULAR_SPRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraints::tabulatedAxialAngularSpring
// Description
//   sixDoFRigidBodyMotionRestraints model.  Axial angular spring with moment
//   values drawn from an interpolation table.  Linear damping.
// SourceFiles
//   tabulated_axial_angular_spring.cpp
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "point.hpp"
#include "tensor.hpp"
#include "interpolation_table.hpp"
namespace mousse
{
namespace sixDoFRigidBodyMotionRestraints
{
class tabulatedAxialAngularSpring
:
  public sixDoFRigidBodyMotionRestraint
{
  // Private data
    //- Reference orientation where there is no moment
    tensor refQ_;
    //- Global unit axis around which the motion is sprung
    vector axis_;
    //- Spring moment interpolation table, depending on angleFormat
    interpolationTable<scalar> moment_;
    //- Boolean stating whether the angle around the axis needs to
    //  be converted to degrees before asking the
    //  interpolationTable for a moment value
    bool convertToDegrees_;
    //- Damping coefficient (Nms/rad)
    scalar damping_;
public:
  //- Runtime type information
  TYPE_NAME("tabulatedAxialAngularSpring");
  // Constructors
    //- Construct from components
    tabulatedAxialAngularSpring
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const
    {
      return autoPtr<sixDoFRigidBodyMotionRestraint>
      (
        new tabulatedAxialAngularSpring(*this)
      );
    }
  //- Destructor
  virtual ~tabulatedAxialAngularSpring();
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
