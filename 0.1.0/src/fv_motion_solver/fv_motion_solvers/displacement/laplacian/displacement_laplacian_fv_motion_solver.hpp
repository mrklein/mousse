// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementLaplacianFvMotionSolver
// Description
//   Mesh motion solver for an fvMesh.  Based on solving the cell-centre
//   Laplacian for the motion displacement.
// SourceFiles
//   displacement_laplacian_fv_motion_solver.cpp
#ifndef displacement_laplacian_fv_motion_solver_hpp_
#define displacement_laplacian_fv_motion_solver_hpp_
#include "displacement_motion_solver.hpp"
#include "fv_motion_solver_core.hpp"
namespace mousse
{
// Forward class declarations
class motionDiffusivity;
class displacementLaplacianFvMotionSolver
:
  public displacementMotionSolver,
  public fvMotionSolverCore
{
  // Private data
    //- Cell-centre motion field
    mutable volVectorField cellDisplacement_;
    //- Optionally read point-position field. Used only for position
    //  boundary conditions.
    mutable autoPtr<pointVectorField> pointLocation_;
    //- Diffusivity used to control the motion
    autoPtr<motionDiffusivity> diffusivityPtr_;
    //- Frozen points (that are not on patches). -1 or points that are
    //  fixed to be at points0_ location
    label frozenPointsZone_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    displacementLaplacianFvMotionSolver
    (
      const displacementLaplacianFvMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const displacementLaplacianFvMotionSolver&);
public:
  //- Runtime type information
  TypeName("displacementLaplacian");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementLaplacianFvMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
  //- Destructor
  ~displacementLaplacianFvMotionSolver();
  // Member Functions
    //- Return reference to the cell motion displacement field
    volVectorField& cellDisplacement()
    {
      return cellDisplacement_;
    }
    //- Return const reference to the cell motion displacement field
    const volVectorField& cellDisplacement() const
    {
      return cellDisplacement_;
    }
    //- Return reference to the diffusivity field
    motionDiffusivity& diffusivity();
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve();
    //- Update topology
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
