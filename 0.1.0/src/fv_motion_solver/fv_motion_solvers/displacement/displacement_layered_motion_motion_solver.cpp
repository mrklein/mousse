// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_layered_motion_motion_solver.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_edge_structured_walk.hpp"
#include "point_fields.hpp"
#include "point_edge_wave.hpp"
#include "sync_tools.hpp"
#include "interpolation_table.hpp"
#include "map_poly_mesh.hpp"
#include "point_constraints.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(displacementLayeredMotionMotionSolver, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionSolver,
  displacementLayeredMotionMotionSolver,
  dictionary
);

}


// Private Member Functions 
void mousse::displacementLayeredMotionMotionSolver::calcZoneMask
(
  const label cellZoneI,
  PackedBoolList& isZonePoint,
  PackedBoolList& isZoneEdge
) const
{
  if (cellZoneI == -1) {
    isZonePoint.setSize(mesh().nPoints());
    isZonePoint = 1;
    isZoneEdge.setSize(mesh().nEdges());
    isZoneEdge = 1;
  } else {
    const cellZone& cz = mesh().cellZones()[cellZoneI];
    label nPoints = 0;
    FOR_ALL(cz, i) {
      const labelList& cPoints = mesh().cellPoints(cz[i]);
      FOR_ALL(cPoints, cPointI) {
        if (!isZonePoint[cPoints[cPointI]]) {
          isZonePoint[cPoints[cPointI]] = 1;
          nPoints++;
        }
      }
    }
    syncTools::syncPointList
      (
        mesh(),
        isZonePoint,
        orEqOp<unsigned int>(),
        0
      );
    // Mark edge inside cellZone
    label nEdges = 0;
    FOR_ALL(cz, i) {
      const labelList& cEdges = mesh().cellEdges(cz[i]);
      FOR_ALL(cEdges, cEdgeI) {
        if (!isZoneEdge[cEdges[cEdgeI]]) {
          isZoneEdge[cEdges[cEdgeI]] = 1;
          nEdges++;
        }
      }
    }
    syncTools::syncEdgeList
      (
        mesh(),
        isZoneEdge,
        orEqOp<unsigned int>(),
        0
      );
    if (debug) {
      Info << "On cellZone " << cz.name()
        << " marked " << returnReduce(nPoints, sumOp<label>())
        << " points and " << returnReduce(nEdges, sumOp<label>())
        << " edges." << endl;
    }
  }
}


// Find distance to starting point
void mousse::displacementLayeredMotionMotionSolver::walkStructured
(
  const label /*cellZoneI*/,
  const PackedBoolList& isZonePoint,
  const PackedBoolList& isZoneEdge,
  const labelList& seedPoints,
  const vectorField& seedData,
  scalarField& distance,
  vectorField& data
) const
{
  List<pointEdgeStructuredWalk> seedInfo{seedPoints.size()};
  FOR_ALL(seedPoints, i) {
    seedInfo[i] =
      pointEdgeStructuredWalk
      (
        points0()[seedPoints[i]],  // location of data
        points0()[seedPoints[i]],  // previous location
        0.0,
        seedData[i]
      );
  }
  // Current info on points
  List<pointEdgeStructuredWalk> allPointInfo{mesh().nPoints()};
  // Mark points inside cellZone.
  // Note that we use points0, not mesh.points()
  // so as not to accumulate errors.
  FOR_ALL(isZonePoint, pointI) {
    if (isZonePoint[pointI]) {
      allPointInfo[pointI] =
        pointEdgeStructuredWalk
        (
          points0()[pointI],  // location of data
          vector::max,        // not valid
          0.0,
          vector::zero        // passive data
        );
    }
  }
  // Current info on edges
  List<pointEdgeStructuredWalk> allEdgeInfo{mesh().nEdges()};
  // Mark edges inside cellZone
  FOR_ALL(isZoneEdge, edgeI) {
    if (isZoneEdge[edgeI]) {
      allEdgeInfo[edgeI] =
        pointEdgeStructuredWalk
        (
          mesh().edges()[edgeI].centre(points0()),    // location of data
          vector::max,                                // not valid
          0.0,
          vector::zero
        );
    }
  }
  // Walk
  PointEdgeWave<pointEdgeStructuredWalk> wallCalc
  {
    mesh(),
    seedPoints,
    seedInfo,
    allPointInfo,
    allEdgeInfo,
    mesh().globalData().nTotalPoints()  // max iterations
  };
  // Extract distance and passive data
  FOR_ALL(allPointInfo, pointI) {
    if (isZonePoint[pointI]) {
      distance[pointI] = allPointInfo[pointI].dist();
      data[pointI] = allPointInfo[pointI].data();
    }
  }
}


// Evaluate faceZone patch
mousse::tmp<mousse::vectorField>
mousse::displacementLayeredMotionMotionSolver::faceZoneEvaluate
(
  const faceZone& fz,
  const labelList& meshPoints,
  const dictionary& dict,
  const PtrList<pointVectorField>& patchDisp,
  const label patchI
) const
{
  tmp<vectorField> tfld{new vectorField(meshPoints.size())};
  vectorField& fld = tfld();
  const word type{dict.lookup("type")};
  if (type == "fixedValue") {
    fld = vectorField("value", dict, meshPoints.size());
  } else if (type == "timeVaryingUniformFixedValue") {
    interpolationTable<vector> timeSeries(dict);
    fld = timeSeries(mesh().time().timeOutputValue());
  } else if (type == "slip") {
    if ((patchI % 2) != 1) {
      FATAL_IO_ERROR_IN
      (
        "displacementLayeredMotionMotionSolver::faceZoneEvaluate"
        "("
          "const faceZone&, "
          "const labelList&, "
          "const dictionary&, "
          "const PtrList<pointVectorField>&, "
          "const label"
        ") const",
        *this
      )
      << "slip can only be used on second faceZone patch of pair.  "
      << "FaceZone:" << fz.name()
      << exit(FatalIOError);
    }
    // Use field set by previous bc
    fld = vectorField(patchDisp[patchI - 1], meshPoints);
  } else if (type == "follow") {
    // Only on boundary faces - follow boundary conditions
    fld = vectorField{pointDisplacement_, meshPoints};
  } else if (type == "uniformFollow") {
    // Reads name of name of patch. Then get average point dislacement on
    // patch. That becomes the value of fld.
    const word patchName{dict.lookup("patch")};
    label patchID = mesh().boundaryMesh().findPatchID(patchName);
    pointField pdf
    {
      pointDisplacement_.boundaryField()[patchID].patchInternalField()
    };
    fld = gAverage(pdf);
  } else {
    FATAL_IO_ERROR_IN
    (
      "displacementLayeredMotionMotionSolver::faceZoneEvaluate"
      "("
      "  const faceZone&, "
      "  const labelList&, "
      "  const dictionary&, "
      "  const PtrList<pointVectorField>&, "
      "  const label"
      ") const",
      *this
    )
    << "Unknown faceZonePatch type " << type << " for faceZone "
    << fz.name() << exit(FatalIOError);
  }
  return tfld;
}


void mousse::displacementLayeredMotionMotionSolver::cellZoneSolve
(
  const label cellZoneI,
  const dictionary& zoneDict
)
{
  PackedBoolList isZonePoint{mesh().nPoints()};
  PackedBoolList isZoneEdge{mesh().nEdges()};
  calcZoneMask(cellZoneI, isZonePoint, isZoneEdge);
  const dictionary& patchesDict = zoneDict.subDict("boundaryField");
  if (patchesDict.size() != 2) {
    FATAL_IO_ERROR_IN
    (
      "displacementLayeredMotionMotionSolver::"
      "cellZoneSolve(const label, const dictionary&)",
      *this
    )
    << "Two faceZones (patches) must be specifed per cellZone. "
    << " cellZone:" << cellZoneI
    << " patches:" << patchesDict.toc()
    << exit(FatalIOError);
  }
  PtrList<scalarField> patchDist{patchesDict.size()};
  PtrList<pointVectorField> patchDisp{patchesDict.size()};
  // Allocate the fields
  label patchI = 0;
  FOR_ALL_CONST_ITER(dictionary, patchesDict, patchIter) {
    const word& faceZoneName = patchIter().keyword();
    label zoneI = mesh().faceZones().findZoneID(faceZoneName);
    if (zoneI == -1) {
      FATAL_IO_ERROR_IN
      (
        "displacementLayeredMotionMotionSolver::"
        "cellZoneSolve(const label, const dictionary&)",
        *this
      )
      << "Cannot find faceZone " << faceZoneName
      << endl << "Valid zones are " << mesh().faceZones().names()
      << exit(FatalIOError);
    }
    // Determine the points of the faceZone within the cellZone
    const faceZone& fz = mesh().faceZones()[zoneI];
    patchDist.set(patchI, new scalarField{mesh().nPoints()});
    patchDisp.set
    (
      patchI,
      new pointVectorField
      {
        IOobject
        {
          mesh().cellZones()[cellZoneI].name() + "_" + fz.name(),
          mesh().time().timeName(),
          mesh(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        pointDisplacement_  // to inherit the boundary conditions
      }
    );
    patchI++;
  }
  // 'correctBoundaryConditions'
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Loops over all the faceZones and walks their boundary values
  // Make sure we can pick up bc values from field
  pointDisplacement_.correctBoundaryConditions();
  patchI = 0;
  FOR_ALL_CONST_ITER(dictionary, patchesDict, patchIter) {
    const word& faceZoneName = patchIter().keyword();
    const dictionary& faceZoneDict = patchIter().dict();
    // Determine the points of the faceZone within the cellZone
    const faceZone& fz = mesh().faceZones()[faceZoneName];
    const labelList& fzMeshPoints = fz().meshPoints();
    DynamicList<label> meshPoints{fzMeshPoints.size()};
    FOR_ALL(fzMeshPoints, i) {
      if (isZonePoint[fzMeshPoints[i]]) {
        meshPoints.append(fzMeshPoints[i]);
      }
    }
    // Get initial value for all the faceZone points
    tmp<vectorField> tseed =
      faceZoneEvaluate
      (
        fz,
        meshPoints,
        faceZoneDict,
        patchDisp,
        patchI
      );
    if (debug) {
      Info << "For cellZone:" << cellZoneI
        << " for faceZone:" << fz.name()
        << " nPoints:" << tseed().size()
        << " have patchField:"
        << " max:" << gMax(tseed())
        << " min:" << gMin(tseed())
        << " avg:" << gAverage(tseed())
        << endl;
    }
    // Set distance and transported value
    walkStructured
      (
        cellZoneI,
        isZonePoint,
        isZoneEdge,
        meshPoints,
        tseed,
        patchDist[patchI],
        patchDisp[patchI]
      );
    // Implement real bc.
    patchDisp[patchI].correctBoundaryConditions();
    patchI++;
  }
  // Solve
  // ~~~~~
  if (debug) {
    // Normalised distance
    pointScalarField distance
    {
      IOobject
      {
        mesh().cellZones()[cellZoneI].name() + ":distance",
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pointMesh::New(mesh()),
      {"zero", dimLength, 0.0}
    };
    FOR_ALL(distance, pointI) {
      scalar d1 = patchDist[0][pointI];
      scalar d2 = patchDist[1][pointI];
      if (d1 + d2 > SMALL) {
        scalar s = d1/(d1 + d2);
        distance[pointI] = s;
      }
    }
    Info << "Writing " << pointScalarField::typeName
      << distance.name() << " to "
      << mesh().time().timeName() << endl;
    distance.write();
  }
  const word interpolationScheme = zoneDict.lookup("interpolationScheme");
  if (interpolationScheme == "oneSided") {
    FOR_ALL(pointDisplacement_, pointI) {
      if (isZonePoint[pointI]) {
        pointDisplacement_[pointI] = patchDisp[0][pointI];
      }
    }
  } else if (interpolationScheme == "linear") {
    FOR_ALL(pointDisplacement_, pointI) {
      if (isZonePoint[pointI]) {
        scalar d1 = patchDist[0][pointI];
        scalar d2 = patchDist[1][pointI];
        scalar s = d1/(d1 + d2 + VSMALL);
        const vector& pd1 = patchDisp[0][pointI];
        const vector& pd2 = patchDisp[1][pointI];
        pointDisplacement_[pointI] = (1 - s)*pd1 + s*pd2;
      }
    }
  } else {
    FATAL_ERROR_IN
    (
      "displacementLayeredMotionMotionSolver::"
      "cellZoneSolve(const label, const dictionary&)"
    )
    << "Invalid interpolationScheme: " << interpolationScheme
    << ". Valid schemes are 'oneSided' and 'linear'"
    << exit(FatalError);
  }
}


// Constructors 
mousse::displacementLayeredMotionMotionSolver::
displacementLayeredMotionMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver(mesh, dict, typeName)
{}


// Destructor 
mousse::displacementLayeredMotionMotionSolver::
~displacementLayeredMotionMotionSolver()
{}


// Member Functions 
mousse::tmp<mousse::pointField>
mousse::displacementLayeredMotionMotionSolver::curPoints() const
{
  tmp<pointField> tcurPoints
  {
    points0() + pointDisplacement_.internalField()
  };
  return tcurPoints;
}


void mousse::displacementLayeredMotionMotionSolver::solve()
{
  // The points have moved so before interpolation update the motionSolver
  movePoints(mesh().points());
  // Apply boundary conditions
  pointDisplacement_.boundaryField().updateCoeffs();
  // Solve motion on all regions (=cellZones)
  const dictionary& regionDicts = coeffDict().subDict("regions");
  FOR_ALL_CONST_ITER(dictionary, regionDicts, regionIter) {
    const word& cellZoneName = regionIter().keyword();
    const dictionary& regionDict = regionIter().dict();
    label zoneI = mesh().cellZones().findZoneID(cellZoneName);
    Info << "solving for zone: " << cellZoneName << endl;
    if (zoneI == -1) {
      FATAL_IO_ERROR_IN
      (
        "displacementLayeredMotionMotionSolver::solve()",
        *this
      )
      << "Cannot find cellZone " << cellZoneName
      << endl << "Valid zones are " << mesh().cellZones().names()
      << exit(FatalIOError);
    }
    cellZoneSolve(zoneI, regionDict);
  }
  // Update pointDisplacement for solved values
  const pointConstraints& pcs =
    pointConstraints::New(pointDisplacement_.mesh());
  pcs.constrainDisplacement(pointDisplacement_, false);
}


void mousse::displacementLayeredMotionMotionSolver::updateMesh
(
  const mapPolyMesh& mpm
)
{
  displacementMotionSolver::updateMesh(mpm);
  const vectorField displacement(this->newPoints() - points0_);
  FOR_ALL(points0_, pointI) {
    label oldPointI = mpm.pointMap()[pointI];
    if (oldPointI >= 0) {
      label masterPointI = mpm.reversePointMap()[oldPointI];
      if ((masterPointI != pointI)) {
        // newly inserted point in this cellZone
        // need to set point0 so that it represents the position that
        // it would have had if it had existed for all time
        points0_[pointI] -= displacement[pointI];
      }
    }
  }
}

