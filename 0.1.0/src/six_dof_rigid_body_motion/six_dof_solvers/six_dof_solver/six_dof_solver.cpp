// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_solver.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(sixDoFSolver, 0);
  defineRunTimeSelectionTable(sixDoFSolver, dictionary);
}
// Constructors 
mousse::sixDoFSolver::sixDoFSolver(sixDoFRigidBodyMotion& body)
:
  body_(body)
{}
// Destructor 
mousse::sixDoFSolver::~sixDoFSolver()
{}
