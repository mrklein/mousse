// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "crank_nicolson.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace sixDoFSolvers {

DEFINE_TYPE_NAME_AND_DEBUG(CrankNicolson, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(sixDoFSolver, CrankNicolson, dictionary);

}
}


// Constructors 
mousse::sixDoFSolvers::CrankNicolson::CrankNicolson
(
  const dictionary& dict,
  sixDoFRigidBodyMotion& body
)
:
  sixDoFSolver{body},
  aoc_{dict.lookupOrDefault<scalar>("aoc", 0.5)},
  voc_{dict.lookupOrDefault<scalar>("voc", 0.5)}
{}


// Destructor 
mousse::sixDoFSolvers::CrankNicolson::~CrankNicolson()
{}


// Member Functions 
void mousse::sixDoFSolvers::CrankNicolson::solve
(
  bool /*firstIter*/,
  const vector& fGlobal,
  const vector& tauGlobal,
  scalar deltaT,
  scalar /*deltaT0*/
)
{
  // Update the linear acceleration and torque
  updateAcceleration(fGlobal, tauGlobal);
  // Correct linear velocity
  v() = tConstraints() & (v0() + aDamp()*deltaT*(aoc_*a() + (1 - aoc_)*a0()));
  // Correct angular momentum
  pi() = rConstraints()
    & (pi0() + aDamp()*deltaT*(aoc_*tau() + (1 - aoc_)*tau0()));
  // Correct position
  centreOfRotation() =
    centreOfRotation0() + deltaT*(voc_*v() + (1 - voc_)*v0());
  // Correct orientation
  Tuple2<tensor, vector> Qpi =
    rotate(Q0(), (voc_*pi() + (1 - voc_)*pi0()), deltaT);
  Q() = Qpi.first();
}

