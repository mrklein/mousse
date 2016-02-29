#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_NEWMARK_NEWMARK_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_NEWMARK_NEWMARK_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFSolvers::Newmark
// Description
//   Newmark 2nd-order time-integrator for 6DoF solid-body motion.
//   Reference:
//   \verbatim
//     Newmark, N. M. (1959).
//     A method of computation for structural dynamics.
//     Journal of the Engineering Mechanics Division, 85(3), 67-94.
//   \endverbatim
//   Example specification in dynamicMeshDict:
//   \verbatim
//   solver
//   {
//     type    Newmark;
//     gamma   0.5;    // Velocity integration coefficient
//     beta    0.25;   // Position integration coefficient
//   }
//   \endverbatim
// SourceFiles
//   newmark.cpp
#include "six_dof_solver.hpp"
namespace mousse
{
namespace sixDoFSolvers
{
class Newmark
:
  public sixDoFSolver
{
  // Private data
    //- Coefficient for velocity integration (default: 0.5)
    const scalar gamma_;
    //- Coefficient for position and orientation integration (default: 0.25)
    const scalar beta_;
public:
  //- Runtime type information
  TYPE_NAME("Newmark");
  // Constructors
    //- Construct from a dictionary and the body
    Newmark
    (
      const dictionary& dict,
      sixDoFRigidBodyMotion& body
    );
  //- Destructor
  virtual ~Newmark();
  // Member Functions
    //- Drag coefficient
    virtual void solve
    (
      bool firstIter,
      const vector& fGlobal,
      const vector& tauGlobal,
      scalar deltaT,
      scalar deltaT0
    );
};
}  // namespace sixDoFSolvers
}  // namespace mousse
#endif
