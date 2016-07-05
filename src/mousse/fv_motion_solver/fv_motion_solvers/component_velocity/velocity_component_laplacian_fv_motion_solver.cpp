// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "velocity_component_laplacian_fv_motion_solver.hpp"
#include "motion_diffusivity.hpp"
#include "fvm_laplacian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_point_interpolation.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(velocityComponentLaplacianFvMotionSolver, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionSolver,
  velocityComponentLaplacianFvMotionSolver,
  dictionary
);

}


// Constructors 
mousse::velocityComponentLaplacianFvMotionSolver::
velocityComponentLaplacianFvMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  componentVelocityMotionSolver{mesh, dict, typeName},
  fvMotionSolverCore{mesh},
  cellMotionU_
  {
    IOobject
    {
      "cellMotionU" + cmptName_,
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    fvMesh_,
    {"cellMotionU", pointMotionU_.dimensions(), 0},
    cellMotionBoundaryTypes<scalar>(pointMotionU_.boundaryField())
  },
  diffusivityPtr_
  {
    motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"))
  }
{}


// Destructor 
mousse::velocityComponentLaplacianFvMotionSolver::
~velocityComponentLaplacianFvMotionSolver()
{}


// Member Functions 
mousse::tmp<mousse::pointField>
mousse::velocityComponentLaplacianFvMotionSolver::curPoints() const
{
  volPointInterpolation::New(fvMesh_)
    .interpolate(cellMotionU_, pointMotionU_);
  tmp<pointField> tcurPoints{new pointField{fvMesh_.points()}};
  const scalar deltaT = fvMesh_.time().deltaTValue();
  tcurPoints().replace
    (
      cmpt_,
      tcurPoints().component(cmpt_) + deltaT*pointMotionU_.internalField()
    );
  twoDCorrectPoints(tcurPoints());
  return tcurPoints;
}


void mousse::velocityComponentLaplacianFvMotionSolver::solve()
{
  // The points have moved so before interpolation update
  // the fvMotionSolver accordingly
  movePoints(fvMesh_.points());
  diffusivityPtr_->correct();
  pointMotionU_.boundaryField().updateCoeffs();
  mousse::solve
    (
      fvm::laplacian
      (
        diffusivityPtr_->operator()(),
        cellMotionU_,
        "laplacian(diffusivity,cellMotionU)"
      )
    );
}


void mousse::velocityComponentLaplacianFvMotionSolver::updateMesh
(
  const mapPolyMesh& mpm
)
{
  componentVelocityMotionSolver::updateMesh(mpm);
  // Update diffusivity. Note two stage to make sure old one is de-registered
  // before creating/registering new one.
  diffusivityPtr_.reset(nullptr);
  diffusivityPtr_ =
    motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"));
}

