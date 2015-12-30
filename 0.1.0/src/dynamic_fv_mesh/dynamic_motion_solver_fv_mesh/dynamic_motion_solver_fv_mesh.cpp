// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_motion_solver_fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "motion_solver.hpp"
#include "vol_fields.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(dynamicMotionSolverFvMesh, 0);
  addToRunTimeSelectionTable
  (
    dynamicFvMesh,
    dynamicMotionSolverFvMesh,
    IOobject
  );
}
// Constructors 
mousse::dynamicMotionSolverFvMesh::dynamicMotionSolverFvMesh(const IOobject& io)
:
  dynamicFvMesh(io),
  motionPtr_(motionSolver::New(*this))
{}
// Destructor 
mousse::dynamicMotionSolverFvMesh::~dynamicMotionSolverFvMesh()
{}
// Member Functions 
bool mousse::dynamicMotionSolverFvMesh::update()
{
  fvMesh::movePoints(motionPtr_->newPoints());
  if (foundObject<volVectorField>("U"))
  {
    volVectorField& U =
      const_cast<volVectorField&>(lookupObject<volVectorField>("U"));
    U.correctBoundaryConditions();
  }
  return true;
}
