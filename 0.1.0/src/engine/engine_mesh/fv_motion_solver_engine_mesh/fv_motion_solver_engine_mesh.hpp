// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMotionSolverEngineMesh
// Description
//   mousse::fvMotionSolverEngineMesh
// SourceFiles
//   fv_motion_solver_engine_mesh.cpp
#ifndef fv_motion_solver_engine_mesh_hpp_
#define fv_motion_solver_engine_mesh_hpp_
#include "engine_mesh.hpp"
#include "velocity_component_laplacian_fv_motion_solver.hpp"
#include "dimensioned_scalar.hpp"
namespace mousse
{
class fvMotionSolverEngineMesh
:
  public engineMesh
{
  // Private data
    dimensionedScalar pistonLayers_;
    //- Mesh-motion solver to solve for the "z" component of the cell-centre
    //  displacements
    velocityComponentLaplacianFvMotionSolver motionSolver_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fvMotionSolverEngineMesh(const fvMotionSolverEngineMesh&);
    //- Disallow default bitwise assignment
    void operator=(const fvMotionSolverEngineMesh&);
public:
  //- Runtime type information
  TypeName("fvMotionSolver");
  // Constructors
    //- Construct from IOobject
    fvMotionSolverEngineMesh(const IOobject& io);
  //- Destructor
  ~fvMotionSolverEngineMesh();
  // Member Functions
    // Edit
      void move();
};
}  // namespace mousse
#endif
