// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicMotionSolverFvMesh
// Description
//   The dynamicMotionSolverFvMesh
// SourceFiles
//   dynamic_motion_solver_fv_mesh.cpp
#ifndef dynamic_motion_solver_fv_mesh_hpp_
#define dynamic_motion_solver_fv_mesh_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    dynamicMotionSolverFvMesh(const dynamicMotionSolverFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const dynamicMotionSolverFvMesh&);
public:
  //- Runtime type information
  TypeName("dynamicMotionSolverFvMesh");
  // Constructors
    //- Construct from IOobject
    dynamicMotionSolverFvMesh(const IOobject& io);
  //- Destructor
  ~dynamicMotionSolverFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
