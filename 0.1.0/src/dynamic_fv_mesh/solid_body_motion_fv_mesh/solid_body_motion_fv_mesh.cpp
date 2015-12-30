// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_body_motion_fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "transform_field.hpp"
#include "cell_zone_mesh.hpp"
#include "cell_set.hpp"
#include "bool_list.hpp"
#include "sync_tools.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(solidBodyMotionFvMesh, 0);
  addToRunTimeSelectionTable(dynamicFvMesh, solidBodyMotionFvMesh, IOobject);
}
// Constructors 
mousse::solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject& io)
:
  dynamicFvMesh(io),
  dynamicMeshCoeffs_
  (
    IOdictionary
    (
      IOobject
      (
        "dynamicMeshDict",
        io.time().constant(),
        *this,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    ).subDict(typeName + "Coeffs")
  ),
  SBMFPtr_(solidBodyMotionFunction::New(dynamicMeshCoeffs_, io.time())),
  undisplacedPoints_
  (
    IOobject
    (
      "points",
      io.time().constant(),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    )
  ),
  pointIDs_(),
  moveAllCells_(false),
  UName_(dynamicMeshCoeffs_.lookupOrDefault<word>("UName", "U"))
{
  if (undisplacedPoints_.size() != nPoints())
  {
    FatalIOErrorIn
    (
      "solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject&)",
      dynamicMeshCoeffs_
    )   << "Read " << undisplacedPoints_.size()
      << " undisplaced points from " << undisplacedPoints_.objectPath()
      << " but the current mesh has " << nPoints()
      << exit(FatalIOError);
  }
  word cellZoneName =
    dynamicMeshCoeffs_.lookupOrDefault<word>("cellZone", "none");
  word cellSetName =
    dynamicMeshCoeffs_.lookupOrDefault<word>("cellSet", "none");
  if ((cellZoneName != "none") && (cellSetName != "none"))
  {
    FatalIOErrorIn
    (
      "solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject&)",
      dynamicMeshCoeffs_
    )
      << "Either cellZone OR cellSet can be supplied, but not both. "
      << "If neither is supplied, all cells will be included"
      << exit(FatalIOError);
  }
  labelList cellIDs;
  if (cellZoneName != "none")
  {
    Info<< "Applying solid body motion to cellZone " << cellZoneName
      << endl;
    label zoneID = cellZones().findZoneID(cellZoneName);
    if (zoneID == -1)
    {
      FatalErrorIn
      (
        "solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject&)"
      )   << "Unable to find cellZone " << cellZoneName
        << ".  Valid cellZones are:"
        << cellZones().names()
        << exit(FatalError);
    }
    cellIDs = cellZones()[zoneID];
  }
  if (cellSetName != "none")
  {
    Info<< "Applying solid body motion to cellSet " << cellSetName
      << endl;
    cellSet set(*this, cellSetName);
    cellIDs = set.toc();
  }
  label nCells = returnReduce(cellIDs.size(), sumOp<label>());
  moveAllCells_ = nCells == 0;
  if (moveAllCells_)
  {
    Info<< "Applying solid body motion to entire mesh" << endl;
  }
  else
  {
    // collect point IDs of points in cell zone
    boolList movePts(nPoints(), false);
    forAll(cellIDs, i)
    {
      label cellI = cellIDs[i];
      const cell& c = cells()[cellI];
      forAll(c, j)
      {
        const face& f = faces()[c[j]];
        forAll(f, k)
        {
          label pointI = f[k];
          movePts[pointI] = true;
        }
      }
    }
    syncTools::syncPointList(*this, movePts, orEqOp<bool>(), false);
    DynamicList<label> ptIDs(nPoints());
    forAll(movePts, i)
    {
      if (movePts[i])
      {
        ptIDs.append(i);
      }
    }
    pointIDs_.transfer(ptIDs);
  }
}
// Destructor 
mousse::solidBodyMotionFvMesh::~solidBodyMotionFvMesh()
{}
// Member Functions 
bool mousse::solidBodyMotionFvMesh::update()
{
  static bool hasWarned = false;
  if (moveAllCells_)
  {
    fvMesh::movePoints
    (
      transform
      (
        SBMFPtr_().transformation(),
        undisplacedPoints_
      )
    );
  }
  else
  {
    pointField transformedPts(undisplacedPoints_);
    UIndirectList<point>(transformedPts, pointIDs_) =
      transform
      (
        SBMFPtr_().transformation(),
        pointField(transformedPts, pointIDs_)
      );
    fvMesh::movePoints(transformedPts);
  }
  if (foundObject<volVectorField>(UName_))
  {
    const volVectorField& U = lookupObject<volVectorField>(UName_);
    const_cast<volVectorField&>(U).correctBoundaryConditions();
  }
  else if (!hasWarned)
  {
    hasWarned = true;
    WarningIn("solidBodyMotionFvMesh::update()")
      << "Did not find volVectorField " << UName_
      << " Not updating " << UName_ << "boundary conditions."
      << endl;
  }
  return true;
}
