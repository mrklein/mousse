// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_component_laplacian_fv_motion_solver.hpp"
#include "motion_diffusivity.hpp"
#include "fvm_laplacian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map_poly_mesh.hpp"
#include "vol_point_interpolation.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(displacementComponentLaplacianFvMotionSolver, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    motionSolver,
    displacementComponentLaplacianFvMotionSolver,
    dictionary
  );
}
// Constructors 
mousse::displacementComponentLaplacianFvMotionSolver::
displacementComponentLaplacianFvMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  componentDisplacementMotionSolver(mesh, dict, type()),
  fvMotionSolverCore(mesh),
  cellDisplacement_
  (
    IOobject
    (
      "cellDisplacement" + cmptName_,
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    ),
    fvMesh_,
    dimensionedScalar
    (
      "cellDisplacement",
      pointDisplacement_.dimensions(),
      0
    ),
    cellMotionBoundaryTypes<scalar>(pointDisplacement_.boundaryField())
  ),
  pointLocation_(NULL),
  diffusivityPtr_
  (
    motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"))
  ),
  frozenPointsZone_
  (
    coeffDict().found("frozenPointsZone")
   ? fvMesh_.pointZones().findZoneID(coeffDict().lookup("frozenPointsZone"))
   : -1
  )
{
  Switch applyPointLocation
  (
    coeffDict().lookupOrDefault
    (
      "applyPointLocation",
      true
    )
  );
  if (applyPointLocation)
  {
    pointLocation_.reset
    (
      new pointVectorField
      (
        IOobject
        (
          "pointLocation",
          fvMesh_.time().timeName(),
          fvMesh_,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        ),
        pointMesh::New(fvMesh_)
      )
    );
    //if (debug)
    {
      Info<< "displacementComponentLaplacianFvMotionSolver :"
        << " Read pointVectorField "
        << pointLocation_().name()
        << " to be used for boundary conditions on points."
        << nl
        << "Boundary conditions:"
        << pointLocation_().boundaryField().types() << endl;
    }
  }
}
// Destructor 
mousse::displacementComponentLaplacianFvMotionSolver::
~displacementComponentLaplacianFvMotionSolver()
{}
// Member Functions 
mousse::tmp<mousse::pointField>
mousse::displacementComponentLaplacianFvMotionSolver::curPoints() const
{
  volPointInterpolation::New(fvMesh_).interpolate
  (
    cellDisplacement_,
    pointDisplacement_
  );
  if (pointLocation_.valid())
  {
    if (debug)
    {
      Info<< "displacementComponentLaplacianFvMotionSolver : applying "
        << " boundary conditions on " << pointLocation_().name()
        << " to new point location."
        << endl;
    }
    // Apply pointLocation_ b.c. to mesh points.
    pointLocation_().internalField() = fvMesh_.points();
    pointLocation_().internalField().replace
    (
      cmpt_,
      points0_ + pointDisplacement_.internalField()
    );
    pointLocation_().correctBoundaryConditions();
    // Implement frozen points
    if (frozenPointsZone_ != -1)
    {
      const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];
      FOR_ALL(pz, i)
      {
        label pointI = pz[i];
        pointLocation_()[pointI][cmpt_] = points0_[pointI];
      }
    }
    twoDCorrectPoints(pointLocation_().internalField());
    return tmp<pointField>(pointLocation_().internalField());
  }
  else
  {
    tmp<pointField> tcurPoints(new pointField(fvMesh_.points()));
    tcurPoints().replace
    (
      cmpt_,
      points0_ + pointDisplacement_.internalField()
    );
    // Implement frozen points
    if (frozenPointsZone_ != -1)
    {
      const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];
      FOR_ALL(pz, i)
      {
        label pointI = pz[i];
        tcurPoints()[pointI][cmpt_] = points0_[pointI];
      }
    }
    twoDCorrectPoints(tcurPoints());
    return tcurPoints;
  }
}
void mousse::displacementComponentLaplacianFvMotionSolver::solve()
{
  // The points have moved so before interpolation update
  // the motionSolver accordingly
  movePoints(fvMesh_.points());
  diffusivityPtr_->correct();
  pointDisplacement_.boundaryField().updateCoeffs();
  mousse::solve
  (
    fvm::laplacian
    (
      diffusivityPtr_->operator()(),
      cellDisplacement_,
      "laplacian(diffusivity,cellDisplacement)"
    )
  );
}
void mousse::displacementComponentLaplacianFvMotionSolver::updateMesh
(
  const mapPolyMesh& mpm
)
{
  componentDisplacementMotionSolver::updateMesh(mpm);
  // Update diffusivity. Note two stage to make sure old one is de-registered
  // before creating/registering new one.
  diffusivityPtr_.reset(NULL);
  diffusivityPtr_ = motionDiffusivity::New
  (
    fvMesh_,
    coeffDict().lookup("diffusivity")
  );
}
