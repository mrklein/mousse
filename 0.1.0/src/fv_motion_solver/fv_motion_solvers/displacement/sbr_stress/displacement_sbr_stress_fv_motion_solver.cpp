// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_sbr_stress_fv_motion_solver.hpp"
#include "motion_diffusivity.hpp"
#include "fvm_laplacian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_div.hpp"
#include "fvc_grad.hpp"
#include "surface_interpolate.hpp"
#include "fvc_laplacian.hpp"
#include "map_poly_mesh.hpp"
#include "vol_point_interpolation.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(displacementSBRStressFvMotionSolver, 0);
  addToRunTimeSelectionTable
  (
    motionSolver,
    displacementSBRStressFvMotionSolver,
    dictionary
  );
}
// Constructors 
mousse::displacementSBRStressFvMotionSolver::displacementSBRStressFvMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver(mesh, dict, dict.lookup("solver")),
  fvMotionSolverCore(mesh),
  cellDisplacement_
  (
    IOobject
    (
      "cellDisplacement",
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    ),
    fvMesh_,
    dimensionedVector
    (
      "cellDisplacement",
      pointDisplacement().dimensions(),
      vector::zero
    ),
    cellMotionBoundaryTypes<vector>(pointDisplacement().boundaryField())
  ),
  diffusivityPtr_
  (
    motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"))
  )
{}
// Destructor 
mousse::displacementSBRStressFvMotionSolver::
~displacementSBRStressFvMotionSolver()
{}
// Member Functions 
mousse::tmp<mousse::pointField>
mousse::displacementSBRStressFvMotionSolver::curPoints() const
{
  volPointInterpolation::New(fvMesh_).interpolate
  (
    cellDisplacement_,
    pointDisplacement_
  );
  tmp<pointField> tcurPoints
  (
    points0() + pointDisplacement().internalField()
  );
  twoDCorrectPoints(tcurPoints());
  return tcurPoints;
}
void mousse::displacementSBRStressFvMotionSolver::solve()
{
  // The points have moved so before interpolation update
  // the mtionSolver accordingly
  movePoints(fvMesh_.points());
  diffusivityPtr_->correct();
  pointDisplacement_.boundaryField().updateCoeffs();
  surfaceScalarField Df(diffusivityPtr_->operator()());
  volTensorField gradCd("gradCd", fvc::grad(cellDisplacement_));
  mousse::solve
  (
    fvm::laplacian
    (
      2*Df,
      cellDisplacement_,
      "laplacian(diffusivity,cellDisplacement)"
    )
   + fvc::div
    (
      Df
     *(
       (
         cellDisplacement_.mesh().Sf()
        & fvc::interpolate(gradCd.T() - gradCd)
       )
       // Solid-body rotation "lambda" term
      - cellDisplacement_.mesh().Sf()*fvc::interpolate(tr(gradCd))
      )
    )
    /*
   - fvc::laplacian
    (
      2*Df,
      cellDisplacement_,
      "laplacian(diffusivity,cellDisplacement)"
    )
   + fvc::div
    (
      Df
     *(
       (
         cellDisplacement_.mesh().Sf()
        & fvc::interpolate(gradCd + gradCd.T())
       )
       // Solid-body rotation "lambda" term
      - cellDisplacement_.mesh().Sf()*fvc::interpolate(tr(gradCd))
     )
    )
    */
  );
}
void mousse::displacementSBRStressFvMotionSolver::updateMesh
(
  const mapPolyMesh& mpm
)
{
  displacementMotionSolver::updateMesh(mpm);
  // Update diffusivity. Note two stage to make sure old one is de-registered
  // before creating/registering new one.
  diffusivityPtr_.reset(NULL);
  diffusivityPtr_ = motionDiffusivity::New
  (
    fvMesh_,
    coeffDict().lookup("diffusivity")
  );
}
