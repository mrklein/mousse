#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_CRANK_NICOLSON_CRANK_NICOLSON_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_CRANK_NICOLSON_CRANK_NICOLSON_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFSolvers::CrankNicolson
// Description
//   Crank-Nicolson 2nd-order time-integrator for 6DoF solid-body motion.
//   The off-centering coefficients for acceleration (velocity integration) and
//   velocity (position/orientation integration) may be specified but default
//   values of 0.5 for each are used if they are not specified.  With the default
//   off-centering this scheme is equivalent to the Newmark scheme with default
//   coefficients.
//   Example specification in dynamicMeshDict:
//   \verbatim
//   solver
//   {
//     type    CrankNicolson;
//     aoc     0.5;    // Acceleration off-centering coefficient
//     voc     0.5;    // Velocity off-centering coefficient
//   }
//   \endverbatim
// SeeAlso
//   mousse::sixDoFSolvers::Newmark

#include "six_dof_solver.hpp"


namespace mousse {
namespace sixDoFSolvers {

class CrankNicolson
:
  public sixDoFSolver
{
  // Private data
    //- Acceleration off-centering coefficient (default: 0.5)
    const scalar aoc_;
    //- Velocity off-centering coefficient (default: 0.5)
    const scalar voc_;
public:
  //- Runtime type information
  TYPE_NAME("CrankNicolson");
  // Constructors
    //- Construct from a dictionary and the body
    CrankNicolson
    (
      const dictionary& dict,
      sixDoFRigidBodyMotion& body
    );
  //- Destructor
  virtual ~CrankNicolson();
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

