// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_displacement_point_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "transform_field.hpp"
#include "fv_mesh.hpp"
#include "displacement_motion_solver.hpp"


namespace mousse {

// Static Data Members
template<>
const char*
NamedEnum<surfaceDisplacementPointPatchVectorField::projectMode, 3>::
names[] =
{
  "nearest",
  "pointNormal",
  "fixedNormal"
};

const NamedEnum<surfaceDisplacementPointPatchVectorField::projectMode, 3>
  surfaceDisplacementPointPatchVectorField::projectModeNames_;


// Private Member Functions 
void surfaceDisplacementPointPatchVectorField::calcProjection
(
  vectorField& displacement
) const
{
  const polyMesh& mesh = patch().boundaryMesh().mesh()();
  const pointField& localPoints = patch().localPoints();
  const labelList& meshPoints = patch().meshPoints();
  //const scalar deltaT = mesh.time().deltaTValue();
  // Construct large enough vector in direction of projectDir so
  // we're guaranteed to hit something.
  //- Per point projection vector:
  const scalar projectLen = mag(mesh.bounds().max()-mesh.bounds().min());
  // For case of fixed projection vector:
  vector projectVec{vector::zero};
  if (projectMode_ == FIXEDNORMAL) {
    vector n = projectDir_/mag(projectDir_);
    projectVec = projectLen*n;
  }
  // Get fixed points (bit of a hack)
  const pointZone* zonePtr = nullptr;
  if (frozenPointsZone_.size() > 0) {
    const pointZoneMesh& pZones = mesh.pointZones();
    zonePtr = &pZones[frozenPointsZone_];
    Pout << "surfaceDisplacementPointPatchVectorField : Fixing all "
      << zonePtr->size() << " points in pointZone " << zonePtr->name()
      << endl;
  }
  // Get the starting locations from the motionSolver
  const pointField& points0 =
    mesh.lookupObject<displacementMotionSolver>
    (
      "dynamicMeshDict"
    ).points0();
  pointField start{meshPoints.size()};
  FOR_ALL(start, i) {
    start[i] = points0[meshPoints[i]] + displacement[i];
  }
  label nNotProjected = 0;
  if (projectMode_ == NEAREST) {
    List<pointIndexHit> nearest;
    labelList hitSurfaces;
    surfaces().findNearest
      (
        start,
        scalarField(start.size(), sqr(projectLen)),
        hitSurfaces,
        nearest
      );
    FOR_ALL(nearest, i) {
      if (zonePtr && (zonePtr->whichPoint(meshPoints[i]) >= 0)) {
        // Fixed point. Reset to point0 location.
        displacement[i] = points0[meshPoints[i]] - localPoints[i];
      } else if (nearest[i].hit()) {
        displacement[i] =
          nearest[i].hitPoint() - points0[meshPoints[i]];
      } else {
        nNotProjected++;
        if (debug) {
          Pout << "    point:" << meshPoints[i]
            << " coord:" << localPoints[i]
            << "  did not find any surface within " << projectLen
            << endl;
        }
      }
    }
  } else {
    // Do tests on all points. Combine later on.
    // 1. Check if already on surface
    List<pointIndexHit> nearest;
    {
      labelList nearestSurface;
      surfaces().findNearest
      (
        start,
        scalarField(start.size(), sqr(SMALL)),
        nearestSurface,
        nearest
      );
    }
    // 2. intersection. (combined later on with information from nearest
    // above)
    vectorField projectVecs{start.size(), projectVec};
    if (projectMode_ == POINTNORMAL) {
      projectVecs = projectLen*patch().pointNormals();
    }
    // Knock out any wedge component
    scalarField offset{start.size(), 0.0};
    if (wedgePlane_ >= 0 && wedgePlane_ <= vector::nComponents) {
      FOR_ALL(offset, i) {
        offset[i] = start[i][wedgePlane_];
        start[i][wedgePlane_] = 0;
        projectVecs[i][wedgePlane_] = 0;
      }
    }
    List<pointIndexHit> rightHit;

    {
      labelList rightSurf;
      surfaces().findAnyIntersection
        (
          start,
          start+projectVecs,
          rightSurf,
          rightHit
        );
    }

    List<pointIndexHit> leftHit;

    {
      labelList leftSurf;
      surfaces().findAnyIntersection
        (
          start,
          start-projectVecs,
          leftSurf,
          leftHit
        );
    }

    // 3. Choose either -fixed, nearest, right, left.
    FOR_ALL(displacement, i) {
      if (zonePtr && (zonePtr->whichPoint(meshPoints[i]) >= 0)) {
        // Fixed point. Reset to point0 location.
        displacement[i] = points0[meshPoints[i]] - localPoints[i];
      } else if (nearest[i].hit()) {
        // Found nearest.
        displacement[i] =
          nearest[i].hitPoint() - points0[meshPoints[i]];
      } else {
        pointIndexHit interPt;
        if (rightHit[i].hit()) {
          if (leftHit[i].hit()) {
            if (magSqr(rightHit[i].hitPoint() - start[i])
                < magSqr(leftHit[i].hitPoint() - start[i])) {
              interPt = rightHit[i];
            } else {
              interPt = leftHit[i];
            }
          } else {
            interPt = rightHit[i];
          }
        } else {
          if (leftHit[i].hit()) {
            interPt = leftHit[i];
          }
        } if (interPt.hit()) {
          if (wedgePlane_ >= 0 && wedgePlane_ <= vector::nComponents) {
            interPt.rawPoint()[wedgePlane_] += offset[i];
          }
          displacement[i] = interPt.rawPoint()-points0[meshPoints[i]];
        } else {
          nNotProjected++;
          if (debug) {
            Pout << "    point:" << meshPoints[i]
              << " coord:" << localPoints[i]
              << "  did not find any intersection between"
              << " ray from " << start[i]-projectVecs[i]
              << " to " << start[i]+projectVecs[i] << endl;
          }
        }
      }
    }
  }
  reduce(nNotProjected, sumOp<label>());
  if (nNotProjected > 0) {
    Info << "surfaceDisplacement :"
      << " on patch " << patch().name()
      << " did not project " << nNotProjected
      << " out of " << returnReduce(localPoints.size(), sumOp<label>())
      << " points." << endl;
  }
}


// Constructors 
surfaceDisplacementPointPatchVectorField::
surfaceDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchVectorField{p, iF},
  velocity_{vector::zero},
  projectMode_{NEAREST},
  projectDir_{vector::zero},
  wedgePlane_{-1}
{}


surfaceDisplacementPointPatchVectorField::
surfaceDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchVectorField{p, iF, dict},
  velocity_{dict.lookup("velocity")},
  surfacesDict_{dict.subDict("geometry")},
  projectMode_{projectModeNames_.read(dict.lookup("projectMode"))},
  projectDir_{dict.lookup("projectDirection")},
  wedgePlane_{dict.lookupOrDefault("wedgePlane", -1)},
  frozenPointsZone_{dict.lookupOrDefault("frozenPointsZone", word::null)}
{
  if (velocity_.x() < 0 || velocity_.y() < 0 || velocity_.z() < 0) {
    FATAL_ERROR_IN
    (
      "surfaceDisplacementPointPatchVectorField::\n"
      "surfaceDisplacementPointPatchVectorField\n"
      "(\n"
      "    const pointPatch& p,\n"
      "    const DimensionedField<vector, pointMesh>& iF,\n"
      "    const dictionary& dict\n"
      ")\n"
    )
    << "All components of velocity have to be positive : "
    << velocity_ << nl
    << "Set velocity components to a great value if no clipping"
    << " necessary." << exit(FatalError);
  }
}


surfaceDisplacementPointPatchVectorField::
surfaceDisplacementPointPatchVectorField
(
  const surfaceDisplacementPointPatchVectorField& ppf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchVectorField{ppf, p, iF, mapper},
  velocity_{ppf.velocity_},
  surfacesDict_{ppf.surfacesDict_},
  projectMode_{ppf.projectMode_},
  projectDir_{ppf.projectDir_},
  wedgePlane_{ppf.wedgePlane_},
  frozenPointsZone_{ppf.frozenPointsZone_}
{}


surfaceDisplacementPointPatchVectorField::
surfaceDisplacementPointPatchVectorField
(
  const surfaceDisplacementPointPatchVectorField& ppf
)
:
  fixedValuePointPatchVectorField{ppf},
  velocity_{ppf.velocity_},
  surfacesDict_{ppf.surfacesDict_},
  projectMode_{ppf.projectMode_},
  projectDir_{ppf.projectDir_},
  wedgePlane_{ppf.wedgePlane_},
  frozenPointsZone_{ppf.frozenPointsZone_}
{}


surfaceDisplacementPointPatchVectorField::
surfaceDisplacementPointPatchVectorField
(
  const surfaceDisplacementPointPatchVectorField& ppf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchVectorField{ppf, iF},
  velocity_{ppf.velocity_},
  surfacesDict_{ppf.surfacesDict_},
  projectMode_{ppf.projectMode_},
  projectDir_{ppf.projectDir_},
  wedgePlane_{ppf.wedgePlane_},
  frozenPointsZone_{ppf.frozenPointsZone_}
{}


// Member Functions 
const searchableSurfaces&
surfaceDisplacementPointPatchVectorField::surfaces() const
{
  if (surfacesPtr_.empty()) {
    surfacesPtr_.reset
    (
      new searchableSurfaces
      {
        {
          "abc",                              // dummy name
          db().time().constant(),             // directory
          "triSurface",                       // instance
          db().time(),                        // registry
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        surfacesDict_,
        true                // allow single-region shortcut
      }
    );
  }
  return surfacesPtr_();
}


void surfaceDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  const polyMesh& mesh = patch().boundaryMesh().mesh()();
  vectorField currentDisplacement{this->patchInternalField()};
  // Calculate intersections with surface w.r.t points0.
  vectorField displacement{currentDisplacement};
  calcProjection(displacement);
  // offset wrt current displacement
  vectorField offset{displacement-currentDisplacement};
  // Clip offset to maximum displacement possible: velocity*timestep
  const scalar deltaT = mesh.time().deltaTValue();
  const vector clipVelocity = velocity_*deltaT;
  FOR_ALL(displacement, i) {
    vector& d = offset[i];
    for (direction cmpt = 0; cmpt < vector::nComponents; cmpt++) {
      if (d[cmpt] < 0) {
        d[cmpt] = max(d[cmpt], -clipVelocity[cmpt]);
      } else {
        d[cmpt] = min(d[cmpt], clipVelocity[cmpt]);
      }
    }
  }
  this->operator==(currentDisplacement+offset);
  fixedValuePointPatchVectorField::updateCoeffs();
}


void surfaceDisplacementPointPatchVectorField::write(Ostream& os) const
{
  fixedValuePointPatchVectorField::write(os);
  os.writeKeyword("velocity") << velocity_
    << token::END_STATEMENT << nl;
  os.writeKeyword("geometry") << surfacesDict_
    << token::END_STATEMENT << nl;
  os.writeKeyword("projectMode") << projectModeNames_[projectMode_]
    << token::END_STATEMENT << nl;
  os.writeKeyword("projectDirection") << projectDir_
    << token::END_STATEMENT << nl;
  os.writeKeyword("wedgePlane") << wedgePlane_
    << token::END_STATEMENT << nl;
  if (frozenPointsZone_ != word::null)
  {
    os.writeKeyword("frozenPointsZone") << frozenPointsZone_
      << token::END_STATEMENT << nl;
  }
}


MAKE_POINT_PATCH_TYPE_FIELD
(
  fixedValuePointPatchVectorField,
  surfaceDisplacementPointPatchVectorField
);

}  // namespace mousse

