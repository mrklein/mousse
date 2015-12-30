// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementSBRStressFvMotionSolver
// Description
//   Mesh motion solver for an fvMesh.  Based on solving the cell-centre
//   solid-body rotation stress equations for the motion displacement.
// SourceFiles
//   displacement_sbr_stress_fv_motion_solver.cpp
#ifndef displacement_sbr_stress_fv_motion_solver_hpp_
#define displacement_sbr_stress_fv_motion_solver_hpp_
#include "displacement_motion_solver.hpp"
#include "fv_motion_solver_core.hpp"
namespace mousse
{
// Forward class declarations
class motionDiffusivity;
class displacementSBRStressFvMotionSolver
:
  public displacementMotionSolver,
  public fvMotionSolverCore
{
  // Private data
    //- Cell-centre motion field
    mutable volVectorField cellDisplacement_;
    //- Diffusivity used to control the motion
    autoPtr<motionDiffusivity> diffusivityPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    displacementSBRStressFvMotionSolver
    (
      const displacementSBRStressFvMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const displacementSBRStressFvMotionSolver&);
public:
  //- Runtime type information
  TypeName("displacementSBRStress");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementSBRStressFvMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
  //- Destructor
  ~displacementSBRStressFvMotionSolver();
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
    //- Return diffusivity
    motionDiffusivity& diffusivity()
    {
      return diffusivityPtr_();
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
