#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SIX_DOF_RIGID_BODY_MOTION_RESTRAINT_SIX_DOF_RIGID_BODY_MOTION_RESTRAINT_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_RESTRAINTS_SIX_DOF_RIGID_BODY_MOTION_RESTRAINT_SIX_DOF_RIGID_BODY_MOTION_RESTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraint
// Description
//   Base class for defining restraints for sixDoF motions

#include "time.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "vector.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

// Forward declaration of classes
class sixDoFRigidBodyMotion;


class sixDoFRigidBodyMotionRestraint
{
protected:
  // Protected data
    //- Name of the restraint
    word name_;
    //- Restraint model specific coefficient dictionary
    dictionary sDoFRBMRCoeffs_;
public:
  //- Runtime type information
  TYPE_NAME("sixDoFRigidBodyMotionRestraint");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      sixDoFRigidBodyMotionRestraint,
      dictionary,
      (const word& name, const dictionary& sDoFRBMRDict),
      (name, sDoFRBMRDict)
    );
  // Constructors
    //- Construct from the sDoFRBMRDict dictionary and Time
    sixDoFRigidBodyMotionRestraint
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
    //- Construct and return a clone
    virtual autoPtr<sixDoFRigidBodyMotionRestraint> clone() const = 0;
  // Selectors
    //- Select constructed from the sDoFRBMRDict dictionary and Time
    static autoPtr<sixDoFRigidBodyMotionRestraint> New
    (
      const word& name,
      const dictionary& sDoFRBMRDict
    );
  //- Destructor
  virtual ~sixDoFRigidBodyMotionRestraint();
  // Member Functions
    //- Return the name
    const word& name() const { return name_; }
    //- Calculate the restraint position, force and moment.
    //  Global reference frame vectors.
    virtual void restrain
    (
      const sixDoFRigidBodyMotion& motion,
      vector& restraintPosition,
      vector& restraintForce,
      vector& restraintMoment
    ) const = 0;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& sDoFRBMRDict);
    // Access
      // Return access to sDoFRBMRCoeffs
      inline const dictionary& coeffDict() const
      {
        return sDoFRBMRCoeffs_;
      }
    //- Write
    virtual void write(Ostream&) const = 0;
};

}  // namespace mousse

#endif

