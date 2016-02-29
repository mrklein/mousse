#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_DISPLACEMENT_MESH_MOVER_MOTION_SOLVER_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_DISPLACEMENT_MESH_MOVER_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementMeshMoverMotionSolver
// Description
//   Mesh motion solver for an fvMesh.  Based on solving the cell-centre
//   Laplacian for the motion displacement.
// SourceFiles
//   displacement_mesh_mover_motion_solver.cpp
#include "displacement_motion_solver.hpp"
#include "external_displacement_mesh_mover.hpp"
namespace mousse
{
class displacementMeshMoverMotionSolver
:
  public displacementMotionSolver
{
  // Private data
    mutable autoPtr<externalDisplacementMeshMover> meshMoverPtr_;
public:
  //- Runtime type information
  TYPE_NAME("displacementMeshMover");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementMeshMoverMotionSolver(const polyMesh&, const IOdictionary&);
    //- Disallow default bitwise copy construct
    displacementMeshMoverMotionSolver
    (
      const displacementMeshMoverMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    displacementMeshMoverMotionSolver& operator=
    (
      const displacementMeshMoverMotionSolver&
    ) = delete;
  //- Destructor
  ~displacementMeshMoverMotionSolver();
  // Member Functions
    externalDisplacementMeshMover& meshMover() const;
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve();
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&);
    //- Update topology
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
