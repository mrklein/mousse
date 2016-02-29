#ifndef DYNAMIC_FV_MESH_DYNAMIC_MOTION_SOLVER_FV_MESH_DYNAMIC_MOTION_SOLVER_FV_MESH_HPP_
#define DYNAMIC_FV_MESH_DYNAMIC_MOTION_SOLVER_FV_MESH_DYNAMIC_MOTION_SOLVER_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicMotionSolverFvMesh
// Description
//   The dynamicMotionSolverFvMesh
// SourceFiles
//   dynamic_motion_solver_fv_mesh.cpp
#include "dynamic_fv_mesh.hpp"
namespace mousse
{
class motionSolver;
class dynamicMotionSolverFvMesh
:
  public dynamicFvMesh
{
  // Private data
    autoPtr<motionSolver> motionPtr_;
public:
  //- Runtime type information
  TYPE_NAME("dynamicMotionSolverFvMesh");
  // Constructors
    //- Construct from IOobject
    dynamicMotionSolverFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    dynamicMotionSolverFvMesh(const dynamicMotionSolverFvMesh&) = delete;
    //- Disallow default bitwise assignment
    dynamicMotionSolverFvMesh& operator=
    (
      const dynamicMotionSolverFvMesh&
    ) = delete;
  //- Destructor
  ~dynamicMotionSolverFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
