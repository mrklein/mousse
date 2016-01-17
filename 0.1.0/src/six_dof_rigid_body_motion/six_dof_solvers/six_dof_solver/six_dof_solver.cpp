// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_solver.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(sixDoFSolver, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(sixDoFSolver, dictionary);
}
// Constructors 
mousse::sixDoFSolver::sixDoFSolver(sixDoFRigidBodyMotion& body)
:
  body_(body)
{}
// Destructor 
mousse::sixDoFSolver::~sixDoFSolver()
{}
