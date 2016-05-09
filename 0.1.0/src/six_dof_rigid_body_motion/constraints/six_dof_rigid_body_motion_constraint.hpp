#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINT_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINT_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINTS_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINT_SIX_DOF_RIGID_BODY_MOTION_CONSTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionConstraint
// Description
//   Base class for defining constraints for sixDoF motions

#include "time.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "point.hpp"
#include "point_constraint.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

// Forward declaration of classes
class sixDoFRigidBodyMotion;


class sixDoFRigidBodyMotionConstraint
{
protected:
  // Protected data
    //- Name of the constraint
    word name_;
    //- Constraint model specific coefficient dictionary
    dictionary sDoFRBMCCoeffs_;
    //- Reference to the body motion
    const sixDoFRigidBodyMotion& motion_;
public:
  //- Runtime type information
  TYPE_NAME("sixDoFRigidBodyMotionConstraint");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      sixDoFRigidBodyMotionConstraint,
      dictionary,
      (
        const word& name,
        const dictionary& sDoFRBMCDict,
        const sixDoFRigidBodyMotion& motion
      ),
      (name, sDoFRBMCDict, motion)
    );
  // Constructors
    //- Construct from the sDoFRBMCDict dictionary and Time
    sixDoFRigidBodyMotionConstraint
    (
      const word& name,
      const dictionary& sDoFRBMCDict,
      const sixDoFRigidBodyMotion& motion
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const = 0;
  // Selectors
    //- Select constructed from the sDoFRBMCDict dictionary and Time
    static autoPtr<sixDoFRigidBodyMotionConstraint> New
    (
      const word& name,
      const dictionary& sDoFRBMCDict,
      const sixDoFRigidBodyMotion& motion
    );
  //- Destructor
  virtual ~sixDoFRigidBodyMotionConstraint();
  // Member Functions
    //- Return the name
    const word& name() const { return name_; }
    //- Set the centre of rotation if not the centre of mass
    virtual void setCentreOfRotation(point&) const
    {}
    //- Apply and accumulate translational constraints
    virtual void constrainTranslation(pointConstraint&) const = 0;
    //- Apply and accumulate rotational constraints
    virtual void constrainRotation(pointConstraint&) const = 0;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& sDoFRBMCDict);
    // Access
      // Return access to sDoFRBMCCoeffs
      inline const dictionary& coeffDict() const { return sDoFRBMCCoeffs_; }
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

