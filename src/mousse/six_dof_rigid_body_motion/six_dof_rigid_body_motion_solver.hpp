#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SOLVER_SIX_DOF_RIGID_BODY_MOTION_SOLVER_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SOLVER_SIX_DOF_RIGID_BODY_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionSolver
// Description
//   6-DoF solid-body mesh motion solver for an fvMesh.
//   Applies SLERP interpolation of movement as function of distance to
//   the object surface.

#include "displacement_motion_solver.hpp"
#include "six_dof_rigid_body_motion.hpp"


namespace mousse {

class sixDoFRigidBodyMotionSolver
:
  public displacementMotionSolver
{
  // Private data
    //- Six dof motion object
    sixDoFRigidBodyMotion motion_;
    wordReList patches_;
    //- Patches to integrate forces
    const labelHashSet patchSet_;
    //- Inner morphing distance (limit of solid-body region)
    const scalar di_;
    //- Outer morphing distance (limit of linear interpolation region)
    const scalar do_;
    //- Switch for test-mode in which only the
    //  gravitational body-force is applied
    Switch test_;
    //- Reference density required by the forces object for
    //  incompressible calculations, required if rhoName == rhoInf
    scalar rhoInf_;
    //- Name of density field, optional unless used for an
    //  incompressible simulation, when this needs to be specified
    //  as rhoInf
    word rhoName_;
    //- Current interpolation scale (1 at patches, 0 at distance_)
    pointScalarField scale_;
    //- Current time index (used for updating)
    label curTimeIndex_;
public:
  //- Runtime type information
  TYPE_NAME("sixDoFRigidBodyMotion");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    sixDoFRigidBodyMotionSolver
    (
      const polyMesh&,
      const IOdictionary& dict
    );
    //- Disallow default bitwise copy construct
    sixDoFRigidBodyMotionSolver
    (
      const sixDoFRigidBodyMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    sixDoFRigidBodyMotionSolver& operator=
    (
      const sixDoFRigidBodyMotionSolver&
    ) = delete;
  //- Destructor
  ~sixDoFRigidBodyMotionSolver();
  // Member Functions
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve();
    //- Write state using given format, version and compression
    virtual bool writeObject
    (
      IOstream::streamFormat fmt,
      IOstream::versionNumber ver,
      IOstream::compressionType cmp
    ) const;
    //- Read dynamicMeshDict dictionary
    virtual bool read();
};

}  // namespace mousse

#endif

