// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "newmark.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace sixDoFSolvers
{
  defineTypeNameAndDebug(Newmark, 0);
  addToRunTimeSelectionTable(sixDoFSolver, Newmark, dictionary);
}
}
// Constructors 
mousse::sixDoFSolvers::Newmark::Newmark
(
  const dictionary& dict,
  sixDoFRigidBodyMotion& body
)
:
  sixDoFSolver(body),
  gamma_(dict.lookupOrDefault<scalar>("gamma", 0.5)),
  beta_
  (
    max
    (
      0.25*sqr(gamma_ + 0.5),
      dict.lookupOrDefault<scalar>("beta", 0.25)
    )
  )
{}
// Destructor 
mousse::sixDoFSolvers::Newmark::~Newmark()
{}
// Member Functions 
void mousse::sixDoFSolvers::Newmark::solve
(
  bool firstIter,
  const vector& fGlobal,
  const vector& tauGlobal,
  scalar deltaT,
  scalar deltaT0
)
{
  // Update the linear acceleration and torque
  updateAcceleration(fGlobal, tauGlobal);
  // Correct linear velocity
  v() =
    tConstraints()
   & (v0() + aDamp()*deltaT*(gamma_*a() + (1 - gamma_)*a0()));
  // Correct angular momentum
  pi() =
    rConstraints()
   & (pi0() + aDamp()*deltaT*(gamma_*tau() + (1 - gamma_)*tau0()));
  // Correct position
  centreOfRotation() =
    centreOfRotation0()
   + (
      tConstraints()
     & (
        deltaT*v0()
       + sqr(deltaT)*beta_*a() + sqr(deltaT)*(0.5 - beta_)*a0()
      )
    );
  // Correct orientation
  vector piDeltaT =
    rConstraints()
   & (
      deltaT*pi0()
     + sqr(deltaT)*beta_*tau() + sqr(deltaT)*(0.5 - beta_)*tau0()
    );
  Tuple2<tensor, vector> Qpi = rotate(Q0(), piDeltaT, 1);
  Q() = Qpi.first();
}
