// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_slip_displacement_point_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "transform_field.hpp"
#include "fv_mesh.hpp"
#include "displacement_motion_solver.hpp"
namespace mousse
{
// Static Data Members
template<>
const char*
NamedEnum<surfaceSlipDisplacementPointPatchVectorField::projectMode, 3>::
names[] =
{
  "nearest",
  "pointNormal",
  "fixedNormal"
};
const NamedEnum<surfaceSlipDisplacementPointPatchVectorField::projectMode, 3>
  surfaceSlipDisplacementPointPatchVectorField::projectModeNames_;
// Private Member Functions 
void surfaceSlipDisplacementPointPatchVectorField::calcProjection
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
  vector projectVec(0, 0, 0);
  if (projectMode_ == FIXEDNORMAL)
  {
    vector n = projectDir_/mag(projectDir_);
    projectVec = projectLen*n;
  }
  // Get fixed points (bit of a hack)
  const pointZone* zonePtr = NULL;
  if (frozenPointsZone_.size() > 0)
  {
    const pointZoneMesh& pZones = mesh.pointZones();
    zonePtr = &pZones[frozenPointsZone_];
    Pout<< "surfaceSlipDisplacementPointPatchVectorField : Fixing all "
      << zonePtr->size() << " points in pointZone " << zonePtr->name()
      << endl;
  }
  // Get the starting locations from the motionSolver
  const pointField& points0 = mesh.lookupObject<displacementMotionSolver>
  (
    "dynamicMeshDict"
  ).points0();
  pointField start(meshPoints.size());
  forAll(start, i)
  {
    start[i] = points0[meshPoints[i]] + displacement[i];
  }
  label nNotProjected = 0;
  if (projectMode_ == NEAREST)
  {
    List<pointIndexHit> nearest;
    labelList hitSurfaces;
    surfaces().findNearest
    (
      start,
      scalarField(start.size(), sqr(projectLen)),
      hitSurfaces,
      nearest
    );
    forAll(nearest, i)
    {
      if (zonePtr && (zonePtr->whichPoint(meshPoints[i]) >= 0))
      {
        // Fixed point. Reset to point0 location.
        displacement[i] = points0[meshPoints[i]] - localPoints[i];
      }
      else if (nearest[i].hit())
      {
        displacement[i] =
          nearest[i].hitPoint()
         - points0[meshPoints[i]];
      }
      else
      {
        nNotProjected++;
        if (debug)
        {
          Pout<< "    point:" << meshPoints[i]
            << " coord:" << localPoints[i]
            << "  did not find any surface within " << projectLen
            << endl;
        }
      }
    }
  }
  else
  {
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
    vectorField projectVecs(start.size(), projectVec);
    if (projectMode_ == POINTNORMAL)
    {
      projectVecs = projectLen*patch().pointNormals();
    }
    // Knock out any wedge component
    scalarField offset(start.size(), 0.0);
    if (wedgePlane_ >= 0 && wedgePlane_ < vector::nComponents)
    {
      forAll(offset, i)
      {
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
    forAll(displacement, i)
    {
      if (zonePtr && (zonePtr->whichPoint(meshPoints[i]) >= 0))
      {
        // Fixed point. Reset to point0 location.
        displacement[i] = points0[meshPoints[i]] - localPoints[i];
      }
      else if (nearest[i].hit())
      {
        // Found nearest.
        displacement[i] =
          nearest[i].hitPoint()
         - points0[meshPoints[i]];
      }
      else
      {
        pointIndexHit interPt;
        if (rightHit[i].hit())
        {
          if (leftHit[i].hit())
          {
            if
            (
              magSqr(rightHit[i].hitPoint()-start[i])
             < magSqr(leftHit[i].hitPoint()-start[i])
            )
            {
              interPt = rightHit[i];
            }
            else
            {
              interPt = leftHit[i];
            }
          }
          else
          {
            interPt = rightHit[i];
          }
        }
        else
        {
          if (leftHit[i].hit())
          {
            interPt = leftHit[i];
          }
        }
        if (interPt.hit())
        {
          if (wedgePlane_ >= 0 && wedgePlane_ < vector::nComponents)
          {
            interPt.rawPoint()[wedgePlane_] += offset[i];
          }
          displacement[i] = interPt.rawPoint()-points0[meshPoints[i]];
        }
        else
        {
          nNotProjected++;
          if (debug)
          {
            Pout<< "    point:" << meshPoints[i]
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
  if (nNotProjected > 0)
  {
    Info<< "surfaceSlipDisplacement :"
      << " on patch " << patch().name()
      << " did not project " << nNotProjected
      << " out of " << returnReduce(localPoints.size(), sumOp<label>())
      << " points." << endl;
  }
}
// Constructors 
surfaceSlipDisplacementPointPatchVectorField::
surfaceSlipDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  pointPatchVectorField(p, iF),
  projectMode_(NEAREST),
  projectDir_(vector::zero),
  wedgePlane_(-1)
{}
surfaceSlipDisplacementPointPatchVectorField::
surfaceSlipDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  pointPatchVectorField(p, iF, dict),
  surfacesDict_(dict.subDict("geometry")),
  projectMode_(projectModeNames_.read(dict.lookup("projectMode"))),
  projectDir_(dict.lookup("projectDirection")),
  wedgePlane_(dict.lookupOrDefault("wedgePlane", -1)),
  frozenPointsZone_(dict.lookupOrDefault("frozenPointsZone", word::null))
{}
surfaceSlipDisplacementPointPatchVectorField::
surfaceSlipDisplacementPointPatchVectorField
(
  const surfaceSlipDisplacementPointPatchVectorField& ppf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper&
)
:
  pointPatchVectorField(p, iF),
  surfacesDict_(ppf.surfacesDict_),
  projectMode_(ppf.projectMode_),
  projectDir_(ppf.projectDir_),
  wedgePlane_(ppf.wedgePlane_),
  frozenPointsZone_(ppf.frozenPointsZone_)
{}
surfaceSlipDisplacementPointPatchVectorField::
surfaceSlipDisplacementPointPatchVectorField
(
  const surfaceSlipDisplacementPointPatchVectorField& ppf
)
:
  pointPatchVectorField(ppf),
  surfacesDict_(ppf.surfacesDict_),
  projectMode_(ppf.projectMode_),
  projectDir_(ppf.projectDir_),
  wedgePlane_(ppf.wedgePlane_),
  frozenPointsZone_(ppf.frozenPointsZone_)
{}
surfaceSlipDisplacementPointPatchVectorField::
surfaceSlipDisplacementPointPatchVectorField
(
  const surfaceSlipDisplacementPointPatchVectorField& ppf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  pointPatchVectorField(ppf, iF),
  surfacesDict_(ppf.surfacesDict_),
  projectMode_(ppf.projectMode_),
  projectDir_(ppf.projectDir_),
  wedgePlane_(ppf.wedgePlane_),
  frozenPointsZone_(ppf.frozenPointsZone_)
{}
// Member Functions 
const searchableSurfaces&
surfaceSlipDisplacementPointPatchVectorField::surfaces() const
{
  if (surfacesPtr_.empty())
  {
    surfacesPtr_.reset
    (
      new searchableSurfaces
      (
        IOobject
        (
          "abc",                              // dummy name
          db().time().constant(),             // directory
          "triSurface",                       // instance
          db().time(),                        // registry
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        ),
        surfacesDict_,
        true    // use single region naming shortcut
      )
    );
  }
  return surfacesPtr_();
}
void surfaceSlipDisplacementPointPatchVectorField::evaluate
(
  const Pstream::commsTypes commsType
)
{
  vectorField displacement(this->patchInternalField());
  // Calculate displacement to project points onto surface
  calcProjection(displacement);
  // Get internal field to insert values into
  Field<vector>& iF = const_cast<Field<vector>&>(this->internalField());
  //setInInternalField(iF, motionU);
  setInInternalField(iF, displacement);
  pointPatchVectorField::evaluate(commsType);
}
void surfaceSlipDisplacementPointPatchVectorField::write(Ostream& os) const
{
  pointPatchVectorField::write(os);
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
makePointPatchTypeField
(
  pointPatchVectorField,
  surfaceSlipDisplacementPointPatchVectorField
);
}  // namespace mousse
