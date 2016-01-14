// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::velocityLaplacianFvMotionSolver
// Description
//   Mesh motion solver for an fvMesh.  Based on solving the cell-centre
//   Laplacian for the motion velocity.
// SourceFiles
//   velocity_laplacian_fv_motion_solver.cpp
#ifndef velocity_laplacian_fv_motion_solver_hpp_
#define velocity_laplacian_fv_motion_solver_hpp_
#include "velocity_motion_solver.hpp"
#include "fv_motion_solver_core.hpp"
namespace mousse
{
// Forward class declarations
class motionDiffusivity;
class velocityLaplacianFvMotionSolver
:
  public velocityMotionSolver,
  public fvMotionSolverCore
{
  // Private data
    //- Cell-centre motion field
    mutable volVectorField cellMotionU_;
    //- Diffusivity used to control the motion
    autoPtr<motionDiffusivity> diffusivityPtr_;
public:
  //- Runtime type information
  TYPE_NAME("velocityLaplacian");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    velocityLaplacianFvMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
    //- Disallow default bitwise copy construct
    velocityLaplacianFvMotionSolver
    (
      const velocityLaplacianFvMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    velocityLaplacianFvMotionSolver& operator=
    (
      const velocityLaplacianFvMotionSolver&
    ) = delete;
  //- Destructor
  ~velocityLaplacianFvMotionSolver();
  // Member Functions
    //- Return reference to the cell motion velocity field
    volVectorField& cellMotionU()
    {
      return cellMotionU_;
    }
    //- Return const reference to the cell motion velocity field
    const volVectorField& cellMotionU() const
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
