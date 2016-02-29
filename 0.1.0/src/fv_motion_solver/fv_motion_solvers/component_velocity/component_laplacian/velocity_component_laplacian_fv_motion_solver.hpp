#ifndef FV_MOTION_SOLVER_FV_MOTION_SOLVERS_COMPONENT_VELOCITY_COMPONENT_LAPLACIAN_VELOCITY_COMPONENT_LAPLACIAN_FV_MOTION_SOLVER_HPP_
#define FV_MOTION_SOLVER_FV_MOTION_SOLVERS_COMPONENT_VELOCITY_COMPONENT_LAPLACIAN_VELOCITY_COMPONENT_LAPLACIAN_FV_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::velocityComponentLaplacianFvMotionSolver
// Description
//   Mesh motion solver for an fvMesh.  Based on solving the cell-centre
//   Laplacian for the given component of the motion velocity.
// SourceFiles
//   velocity_component_laplacian_fv_motion_solver.cpp
#include "component_velocity_motion_solver.hpp"
#include "fv_motion_solver_core.hpp"
namespace mousse
{
// Forward class declarations
class motionDiffusivity;
class velocityComponentLaplacianFvMotionSolver
:
  public componentVelocityMotionSolver,
  public fvMotionSolverCore
{
  // Private data
    //- Cell-centre motion field
    mutable volScalarField cellMotionU_;
    //- Diffusivity used to control the motion
    autoPtr<motionDiffusivity> diffusivityPtr_;
public:
  //- Runtime type information
  TYPE_NAME("velocityComponentLaplacian");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    velocityComponentLaplacianFvMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
    //- Disallow default bitwise copy construct
    velocityComponentLaplacianFvMotionSolver
    (
      const velocityComponentLaplacianFvMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    velocityComponentLaplacianFvMotionSolver& operator=
    (
      const velocityComponentLaplacianFvMotionSolver&
    ) = delete;
  //- Destructor
  ~velocityComponentLaplacianFvMotionSolver();
  // Member Functions
    //- Non-const access to the cellMotionU in order to allow changes
    //  to the boundary motion
    volScalarField& cellMotionU()
    {
      return cellMotionU_;
    }
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve();
    //- Update topology
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
