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
#ifndef velocity_component_laplacian_fv_motion_solver_hpp_
#define velocity_component_laplacian_fv_motion_solver_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    velocityComponentLaplacianFvMotionSolver
    (
      const velocityComponentLaplacianFvMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const velocityComponentLaplacianFvMotionSolver&);
public:
  //- Runtime type information
  TypeName("velocityComponentLaplacian");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    velocityComponentLaplacianFvMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
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
