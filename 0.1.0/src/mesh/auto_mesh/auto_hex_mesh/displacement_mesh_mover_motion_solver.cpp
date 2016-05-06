// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_mesh_mover_motion_solver.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "local_point_region.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(displacementMeshMoverMotionSolver, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionSolver,
  displacementMeshMoverMotionSolver,
  dictionary
);

}


// Private Member Functions 
// Constructors 
mousse::displacementMeshMoverMotionSolver::displacementMeshMoverMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver{mesh, dict, typeName} // read pointDisplacement
{}


// Destructor 
mousse::displacementMeshMoverMotionSolver::
~displacementMeshMoverMotionSolver()
{}


// Member Functions 
mousse::externalDisplacementMeshMover&
mousse::displacementMeshMoverMotionSolver::meshMover() const
{
  if (!meshMoverPtr_.valid()) {
    const word moverType(coeffDict().lookup("meshMover"));
    meshMoverPtr_ =
      externalDisplacementMeshMover::New
      (
        moverType,
        coeffDict().subDict(moverType + "Coeffs"),
        localPointRegion::findDuplicateFacePairs(mesh()),
        pointDisplacement_
      );
  }
  return meshMoverPtr_();
}


mousse::tmp<mousse::pointField>
mousse::displacementMeshMoverMotionSolver::curPoints() const
{
  // Return actual points. Cannot do a reference since complains about
  // assignment to self in polyMesh::movePoints
  return tmp<pointField>(new pointField(mesh().points()));
}


void mousse::displacementMeshMoverMotionSolver::solve()
{
  // The points have moved so before calculation update
  // the mesh and motionSolver accordingly
  movePoints(mesh().points());
  // Update any point motion bcs (e.g. timevarying)
  pointDisplacement().boundaryField().updateCoeffs();
  label nAllowableErrors = 0;
  labelList checkFaces{identity(mesh().nFaces())};
  meshMover().move
    (
      coeffDict().subDict(meshMover().type() + "Coeffs"),
      nAllowableErrors,
      checkFaces
    );
  // This will have updated the mesh and implicitly the pointDisplacement
  pointDisplacement().correctBoundaryConditions();
}


void mousse::displacementMeshMoverMotionSolver::movePoints(const pointField& p)
{
  displacementMotionSolver::movePoints(p);
  // Update meshMover for new geometry
  if (meshMoverPtr_.valid()) {
    meshMover().movePoints(p);
  }
}


void mousse::displacementMeshMoverMotionSolver::updateMesh
(
  const mapPolyMesh& map
)
{
  displacementMotionSolver::updateMesh(map);
  // Update meshMover for new topology
  meshMoverPtr_.clear();
}

