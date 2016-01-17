// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_file.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(pointFile, 0);
addToRunTimeSelectionTable(initialPointsMethod, pointFile, dictionary);
// Constructors 
pointFile::pointFile
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
  (
    typeName,
    initialPointsDict,
    runTime,
    rndGen,
    geometryToConformTo,
    cellShapeControls,
    decomposition
  ),
  pointFileName_(detailsDict().lookup("pointFile")),
  insideOutsideCheck_(detailsDict().lookup("insideOutsideCheck")),
  randomiseInitialGrid_(detailsDict().lookup("randomiseInitialGrid")),
  randomPerturbationCoeff_
  (
    readScalar(detailsDict().lookup("randomPerturbationCoeff"))
  )
{
  Info<< "    Inside/Outside check is " << insideOutsideCheck_.asText()
    << endl;
}
// Member Functions 
List<Vb::Point> pointFile::initialPoints() const
{
  pointIOField points
  (
    IOobject
    (
      pointFileName_.name(),
      time().timeName(),
      time(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    )
  );
  Info<< "    Inserting points from file " << pointFileName_ << endl;
  if (points.empty())
  {
    FatalErrorIn("List<Vb::Point> pointFile::initialPoints() const")
      << "Point file contain no points"
      << exit(FatalError) << endl;
  }
  if (Pstream::parRun())
  {
    // Testing filePath to see if the file originated in a processor
    // directory, if so, assume that the points in each processor file
    // are unique.  They are unlikely to belong on the current
    // processor as the background mesh is unlikely to be the same.
    const bool isParentFile = (points.objectPath() != points.filePath());
    if (!isParentFile)
    {
      decomposition().distributePoints(points);
    }
    else
    {
      // Otherwise, this is assumed to be points covering the whole
      // domain, so filter the points to be only those on this processor
      boolList procPt(decomposition().positionOnThisProcessor(points));
      List<boolList> allProcPt(Pstream::nProcs());
      allProcPt[Pstream::myProcNo()] = procPt;
      Pstream::gatherList(allProcPt);
      Pstream::scatterList(allProcPt);
      forAll(procPt, ptI)
      {
        bool foundAlready = false;
        forAll(allProcPt, procI)
        {
          // If a processor with a lower index has found this point
          // to insert already, defer to it and don't insert.
          if (foundAlready)
          {
            allProcPt[procI][ptI] = false;
          }
          else if (allProcPt[procI][ptI])
          {
            foundAlready = true;
          }
        }
      }
      procPt = allProcPt[Pstream::myProcNo()];
      inplaceSubset(procPt, points);
    }
  }
  Field<bool> insidePoints(points.size(), true);
  if (insideOutsideCheck_)
  {
    insidePoints = geometryToConformTo().wellInside
    (
      points,
      minimumSurfaceDistanceCoeffSqr_
     *sqr(cellShapeControls().cellSize(points))
    );
  }
  DynamicList<Vb::Point> initialPoints(insidePoints.size()/10);
  forAll(insidePoints, i)
  {
    if (insidePoints[i])
    {
      point& p = points[i];
      if (randomiseInitialGrid_)
      {
        p.x() += randomPerturbationCoeff_*(rndGen().scalar01() - 0.5);
        p.y() += randomPerturbationCoeff_*(rndGen().scalar01() - 0.5);
        p.z() += randomPerturbationCoeff_*(rndGen().scalar01() - 0.5);
      }
      initialPoints.append(Vb::Point(p.x(), p.y(), p.z()));
    }
  }
  initialPoints.shrink();
  label nPointsRejected = points.size() - initialPoints.size();
  if (Pstream::parRun())
  {
    reduce(nPointsRejected, sumOp<label>());
  }
  if (nPointsRejected)
  {
    Info<< "    " << nPointsRejected << " points rejected from "
      << pointFileName_.name() << endl;
  }
  return initialPoints;
}
}  // namespace mousse
