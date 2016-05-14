// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ray_shooting.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(rayShooting, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(initialPointsMethod, rayShooting, dictionary);


// Private Member Functions 
void rayShooting::splitLine
(
  const line<point, point>& l,
  const scalar& pert,
  DynamicList<Vb::Point>& initialPoints
) const
{
  mousse::point midPoint{l.centre()};
  const scalar localCellSize{cellShapeControls().cellSize(midPoint)};
  const scalar minDistFromSurfaceSqr
  {
    minimumSurfaceDistanceCoeffSqr_*sqr(localCellSize)
  };
  if (magSqr(midPoint - l.start()) > minDistFromSurfaceSqr
      && magSqr(midPoint - l.end()) > minDistFromSurfaceSqr) {
    if (randomiseInitialGrid_) {
      mousse::point newPt
      {
        midPoint.x() + pert*(rndGen().scalar01() - 0.5),
        midPoint.y() + pert*(rndGen().scalar01() - 0.5),
        midPoint.z() + pert*(rndGen().scalar01() - 0.5)
      };
      if (!geometryToConformTo().findSurfaceAnyIntersection
          (
            midPoint,
            newPt
          )) {
        midPoint = newPt;
      } else {
        WARNING_IN
        (
          "rayShooting::splitLine"
          "("
          "  const line<point,point>&,"
          "  const scalar&,"
          "  DynamicList<Vb::Point>&"
          ")"
        )
        << "Point perturbation crosses a surface. Not inserting."
        << endl;
      }
    }
    initialPoints.append(toPoint(midPoint));
  }
}


// Constructors 
rayShooting::rayShooting
(
  const dictionary& initialPointsDict,
  const Time& runTime,
  Random& rndGen,
  const conformationSurfaces& geometryToConformTo,
  const cellShapeControl& cellShapeControls,
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
:
  initialPointsMethod
  {
    typeName,
    initialPointsDict,
    runTime,
    rndGen,
    geometryToConformTo,
    cellShapeControls,
    decomposition
  },
  randomiseInitialGrid_{detailsDict().lookup("randomiseInitialGrid")},
  randomPerturbationCoeff_
  {
    readScalar(detailsDict().lookup("randomPerturbationCoeff"))
  }
{}


// Member Functions 
List<Vb::Point> rayShooting::initialPoints() const
{
  // Loop over surface faces
  const searchableSurfaces& surfaces = geometryToConformTo().geometry();
  const labelList& surfacesToConformTo = geometryToConformTo().surfaces();
  const scalar maxRayLength{surfaces.bounds().mag()};
  // Initialise points list
  label initialPointsSize = 0;
  FOR_ALL(surfaces, surfI) {
    initialPointsSize += surfaces[surfI].size();
  }
  DynamicList<Vb::Point> initialPoints{initialPointsSize};
  FOR_ALL(surfacesToConformTo, surfI) {
    const searchableSurface& s = surfaces[surfacesToConformTo[surfI]];
    tmp<pointField> faceCentresTmp{s.coordinates()};
    const pointField& faceCentres = faceCentresTmp();
    Info << "    Shoot rays from " << s.name() << nl
      << "        nRays = " << faceCentres.size() << endl;
    FOR_ALL(faceCentres, fcI) {
      const mousse::point& fC = faceCentres[fcI];
      if (Pstream::parRun()
          && !decomposition().positionOnThisProcessor(fC)) {
        continue;
      }
      const scalar pert
      {
        randomPerturbationCoeff_*cellShapeControls().cellSize(fC)
      };
      pointIndexHit surfHitStart;
      label hitSurfaceStart;
      // Face centres should be on the surface so search distance can be
      // small
      geometryToConformTo().findSurfaceNearest
      (
        fC,
        sqr(pert),
        surfHitStart,
        hitSurfaceStart
      );
      vectorField normStart{1, vector::min};
      geometryToConformTo().getNormal
      (
        hitSurfaceStart,
        List<pointIndexHit>{1, surfHitStart},
        normStart
      );
      pointIndexHit surfHitEnd;
      label hitSurfaceEnd;
      geometryToConformTo().findSurfaceNearestIntersection
      (
        fC - normStart[0]*pert,
        fC - normStart[0]*maxRayLength,
        surfHitEnd,
        hitSurfaceEnd
      );
      if (surfHitEnd.hit()) {
        vectorField normEnd{1, vector::min};
        geometryToConformTo().getNormal
        (
          hitSurfaceEnd,
          List<pointIndexHit>(1, surfHitEnd),
          normEnd
        );
        if ((normStart[0] & normEnd[0]) < 0) {
          line<point, point> l{fC, surfHitEnd.hitPoint()};
          if (Pstream::parRun()) {
            // Clip the line in parallel
            pointIndexHit procIntersection =
              decomposition().findLine
              (
                l.start(),
                l.end()
              );
            if (procIntersection.hit()) {
              l = line<point, point>{l.start(), procIntersection.hitPoint()};
            }
          }
          splitLine
          (
            l,
            pert,
            initialPoints
          );
        }
      }
    }
  }
  return initialPoints.shrink();
}

}  // namespace mousse

