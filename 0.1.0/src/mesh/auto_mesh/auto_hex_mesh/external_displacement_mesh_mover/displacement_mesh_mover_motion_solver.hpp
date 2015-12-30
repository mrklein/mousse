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
#ifndef displacement_mesh_mover_motion_solver_hpp_
#define displacement_mesh_mover_motion_solver_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    displacementMeshMoverMotionSolver
    (
      const displacementMeshMoverMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const displacementMeshMoverMotionSolver&);
public:
  //- Runtime type information
  TypeName("displacementMeshMover");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementMeshMoverMotionSolver(const polyMesh&, const IOdictionary&);
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
