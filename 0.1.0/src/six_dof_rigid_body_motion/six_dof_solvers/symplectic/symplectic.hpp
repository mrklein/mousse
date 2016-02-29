#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_SYMPLECTIC_SYMPLECTIC_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_SYMPLECTIC_SYMPLECTIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFSolvers::symplectic
// Description
//   Symplectic 2nd-order explicit time-integrator for 6DoF solid-body motion.
//   Reference:
//   \verbatim
//     Dullweber, A., Leimkuhler, B., & McLachlan, R. (1997).
//     Symplectic splitting methods for rigid body molecular dynamics.
//     The Journal of chemical physics, 107(15), 5840-5851.
//   \endverbatim
//   Can only be used for explicit integration of the motion of the body,
//   i.e. may only be called once per time-step, no outer-correctors may be
//   applied.  For implicit integration with outer-correctors choose either
//   CrankNicolson or Newmark schemes.
//   Example specification in dynamicMeshDict:
//   \verbatim
//   solver
//   {
//     type    symplectic;
//   }
//   \endverbatim
// SeeAlso
//   mousse::sixDoFSolvers::CrankNicolson
//   mousse::sixDoFSolvers::Newmark
// SourceFiles
//   symplectic.cpp
#include "six_dof_solver.hpp"
namespace mousse
{
namespace sixDoFSolvers
{
class symplectic
:
  public sixDoFSolver
{
public:
  //- Runtime type information
  TYPE_NAME("symplectic");
  // Constructors
    //- Construct from a dictionary and the body
    symplectic
    (
      const dictionary& dict,
      sixDoFRigidBodyMotion& body
    );
  //- Destructor
  virtual ~symplectic();
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
