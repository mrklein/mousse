// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionRestraint
// Description
//   Base class for defining restraints for sixDoF motions
// SourceFiles
//   six_dof_rigid_body_motion_restraint.cpp
//   six_dof_rigid_body_motion_restraint_new.cpp
#ifndef six_dof_rigid_body_motion_restraint_hpp_
#define six_dof_rigid_body_motion_restraint_hpp_
#include "time.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "vector.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
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
  TypeName("sixDoFRigidBodyMotionRestraint");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
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
    const word& name() const
    {
      return name_;
    }
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
