// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symplectic.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFSolvers
{
  defineTypeNameAndDebug(symplectic, 0);
  addToRunTimeSelectionTable(sixDoFSolver, symplectic, dictionary);
}
}
// Constructors 
mousse::sixDoFSolvers::symplectic::symplectic
(
  const dictionary& dict,
  sixDoFRigidBodyMotion& body
)
:
  sixDoFSolver(body)
{}
// Destructor 
mousse::sixDoFSolvers::symplectic::~symplectic()
{}
// Member Functions 
void mousse::sixDoFSolvers::symplectic::solve
(
  bool firstIter,
  const vector& fGlobal,
  const vector& tauGlobal,
  scalar deltaT,
  scalar deltaT0
)
{
  if (!firstIter)
  {
    FatalErrorIn("sixDoFSolvers::symplectic::solve")
      << "The symplectic integrator is explicit "
       "and can only be solved once per time-step"
      << exit(FatalError);
  }
  // First simplectic step:
  //     Half-step for linear and angular velocities
  //     Update position and orientation
  v() = tConstraints() & (v0() + aDamp()*0.5*deltaT0*a0());
  pi() = rConstraints() & (pi0() + aDamp()*0.5*deltaT0*tau0());
  centreOfRotation() = centreOfRotation0() + deltaT*v();
  Tuple2<tensor, vector> Qpi = rotate(Q0(), pi(), deltaT);
  Q() = Qpi.first();
  pi() = rConstraints() & Qpi.second();
  // Update the linear acceleration and torque
  updateAcceleration(fGlobal, tauGlobal);
  // Second simplectic step:
  //     Complete update of linear and angular velocities
  v() += tConstraints() & aDamp()*0.5*deltaT*a();
  pi() += rConstraints() & aDamp()*0.5*deltaT*tau();
}
