// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "velocity_motion_solver.hpp"
#include "map_poly_mesh.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(velocityMotionSolver, 0);
}
// Constructors 
mousse::velocityMotionSolver::velocityMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict,
  const word& type
)
:
  motionSolver(mesh, dict, type),
  pointMotionU_
  (
    IOobject
    (
      "pointMotionU",
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    pointMesh::New(mesh)
  )
{}
// Destructor 
mousse::velocityMotionSolver::~velocityMotionSolver()
{}
// Member Functions 
void mousse::velocityMotionSolver::movePoints(const pointField&)
{
  // No local data that needs adapting.
}
void mousse::velocityMotionSolver::updateMesh(const mapPolyMesh& mpm)
{
  // pointMesh already updates pointFields.
  motionSolver::updateMesh(mpm);
}
