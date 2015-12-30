// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "component_velocity_motion_solver.hpp"
#include "map_poly_mesh.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(componentVelocityMotionSolver, 0);
}
// Private Member Functions 
mousse::direction mousse::componentVelocityMotionSolver::cmpt
(
  const word& cmptName
) const
{
  if (cmptName == "x")
  {
    return vector::X;
  }
  else if (cmptName == "y")
  {
    return vector::Y;
  }
  else if (cmptName == "z")
  {
    return vector::Z;
  }
  else
  {
    FatalErrorIn
    (
      "componentVelocityMotionSolver::"
      "componentVelocityMotionSolver"
      "(const polyMesh& mesh, const IOdictionary&)"
    )   << "Given component name " << cmptName << " should be x, y or z"
      << exit(FatalError);
    return 0;
  }
}
// Constructors 
mousse::componentVelocityMotionSolver::componentVelocityMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict,
  const word& type
)
:
  motionSolver(mesh, dict, type),
  cmptName_(coeffDict().lookup("component")),
  cmpt_(cmpt(cmptName_)),
  pointMotionU_
  (
    IOobject
    (
      "pointMotionU" + cmptName_,
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    pointMesh::New(mesh)
  )
{}
// Destructor 
mousse::componentVelocityMotionSolver::~componentVelocityMotionSolver()
{}
// Member Functions 
void mousse::componentVelocityMotionSolver::movePoints(const pointField& p)
{
  // No local data to adapt
}
void mousse::componentVelocityMotionSolver::updateMesh(const mapPolyMesh& mpm)
{
  // pointMesh already updates pointFields.
  motionSolver::updateMesh(mpm);
}
