// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_laplacian_fv_motion_solver.hpp"
#include "motion_diffusivity.hpp"
#include "fvm_laplacian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "map_poly_mesh.hpp"
#include "vol_point_interpolation.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(displacementLaplacianFvMotionSolver, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionSolver,
  displacementLaplacianFvMotionSolver,
  dictionary
);

}


// Constructors 
mousse::displacementLaplacianFvMotionSolver::displacementLaplacianFvMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver{mesh, dict, typeName},
  fvMotionSolverCore{mesh},
  cellDisplacement_
  {
    IOobject
    {
      "cellDisplacement",
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    fvMesh_,
    {"cellDisplacement", pointDisplacement_.dimensions(), vector::zero},
    cellMotionBoundaryTypes<vector>(pointDisplacement_.boundaryField())
  },
  pointLocation_(nullptr),
  diffusivityPtr_
  {
    motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"))
  },
  frozenPointsZone_
  {
    coeffDict().found("frozenPointsZone")
    ? fvMesh_.pointZones().findZoneID(coeffDict().lookup("frozenPointsZone"))
    : -1
  }
{
  IOobject io
  {
    "pointLocation",
    fvMesh_.time().timeName(),
    fvMesh_,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  };
  if (debug) {
    Info << "displacementLaplacianFvMotionSolver:" << nl
      << "    diffusivity       : " << diffusivityPtr_().type() << nl
      << "    frozenPoints zone : " << frozenPointsZone_ << endl;
  }
  if (io.headerOk()) {
    pointLocation_.reset
    (
      new pointVectorField
      {
        io,
        pointMesh::New(fvMesh_)
      }
    );
    if (debug) {
      Info << "displacementLaplacianFvMotionSolver :"
        << " Read pointVectorField "
        << io.name()
        << " to be used for boundary conditions on points."
        << nl
        << "Boundary conditions:"
        << pointLocation_().boundaryField().types() << endl;
    }
  }
}


// Destructor 
mousse::displacementLaplacianFvMotionSolver::
~displacementLaplacianFvMotionSolver()
{}


// Member Functions 
mousse::motionDiffusivity&
mousse::displacementLaplacianFvMotionSolver::diffusivity()
{
  if (!diffusivityPtr_.valid()) {
    diffusivityPtr_ =
      motionDiffusivity::New
      (
        fvMesh_,
        coeffDict().lookup("diffusivity")
      );
  }
  return diffusivityPtr_();
}


mousse::tmp<mousse::pointField>
mousse::displacementLaplacianFvMotionSolver::curPoints() const
{
  volPointInterpolation::New(fvMesh_)
    .interpolate(cellDisplacement_, pointDisplacement_);
  if (pointLocation_.valid()) {
    if (debug) {
      Info << "displacementLaplacianFvMotionSolver : applying "
        << " boundary conditions on " << pointLocation_().name()
        << " to new point location."
        << endl;
    }
    pointLocation_().internalField() =
      points0() + pointDisplacement_.internalField();
    pointLocation_().correctBoundaryConditions();
    // Implement frozen points
    if (frozenPointsZone_ != -1) {
      const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];
      FOR_ALL(pz, i) {
        pointLocation_()[pz[i]] = points0()[pz[i]];
      }
    }
    twoDCorrectPoints(pointLocation_().internalField());
    return tmp<pointField>{pointLocation_().internalField()};
  } else {
    tmp<pointField> tcurPoints
    {
      points0() + pointDisplacement_.internalField()
    };
    // Implement frozen points
    if (frozenPointsZone_ != -1) {
      const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];
      FOR_ALL(pz, i) {
        tcurPoints()[pz[i]] = points0()[pz[i]];
      }
    }
    twoDCorrectPoints(tcurPoints());
    return tcurPoints;
  }
}


void mousse::displacementLaplacianFvMotionSolver::solve()
{
  // The points have moved so before interpolation update
  // the motionSolver accordingly
  movePoints(fvMesh_.points());
  diffusivity().correct();
  pointDisplacement_.boundaryField().updateCoeffs();
  mousse::solve
    (
      fvm::laplacian
      (
        diffusivity().operator()(),
        cellDisplacement_,
        "laplacian(diffusivity,cellDisplacement)"
      )
    );
}


void mousse::displacementLaplacianFvMotionSolver::updateMesh
(
  const mapPolyMesh& mpm
)
{
  displacementMotionSolver::updateMesh(mpm);
  // Update diffusivity. Note two stage to make sure old one is de-registered
  // before creating/registering new one.
  diffusivityPtr_.clear();
}

