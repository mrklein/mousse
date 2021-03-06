// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_interpolation_motion_solver.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "sortable_list.hpp"
#include "iolist.hpp"
#include "tuple2.hpp"
#include "map_poly_mesh.hpp"
#include "interpolate_xy.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(displacementInterpolationMotionSolver, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionSolver,
  displacementInterpolationMotionSolver,
  dictionary
);
template<>
const word IOList<Tuple2<scalar, vector>>::typeName("scalarVectorTable");

}


// Private Member Functions 

// Constructors 
mousse::displacementInterpolationMotionSolver::
displacementInterpolationMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver{mesh, dict, typeName}
{
  // Get zones and their interpolation tables for displacement
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  List<Pair<word>> faceZoneToTable
  {
    coeffDict().lookup("interpolationTables")
  };
  const faceZoneMesh& fZones = mesh.faceZones();
  times_.setSize(fZones.size());
  displacements_.setSize(fZones.size());
  FOR_ALL(faceZoneToTable, i) {
    const word& zoneName = faceZoneToTable[i][0];
    label zoneI = fZones.findZoneID(zoneName);
    if (zoneI == -1) {
      FATAL_ERROR_IN
      (
        "displacementInterpolationMotionSolver::"
        "displacementInterpolationMotionSolver(const polyMesh&,"
        "Istream&)"
      )
      << "Cannot find zone " << zoneName << endl
      << "Valid zones are " << mesh.faceZones().names()
      << exit(FatalError);
    }
    const word& tableName = faceZoneToTable[i][1];
    IOList<Tuple2<scalar, vector>> table
    {
      IOobject
      {
        tableName,
        mesh.time().constant(),
        "tables",
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      }
    };
    // Copy table
    times_[zoneI].setSize(table.size());
    displacements_[zoneI].setSize(table.size());
    FOR_ALL(table, j) {
      times_[zoneI][j] = table[j].first();
      displacements_[zoneI][j] = table[j].second();
    }
  }
  // Sort points into bins according to position relative to faceZones
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Done in all three directions.
  for (direction dir = 0; dir < vector::nComponents; dir++) {
    // min and max coordinates of all faceZones
    SortableList<scalar> zoneCoordinates{2*faceZoneToTable.size()};
    FOR_ALL(faceZoneToTable, i) {
      const word& zoneName = faceZoneToTable[i][0];
      const faceZone& fz = fZones[zoneName];
      scalar minCoord = VGREAT;
      scalar maxCoord = -VGREAT;
      FOR_ALL(fz().meshPoints(), localI) {
        label pointI = fz().meshPoints()[localI];
        const scalar coord = points0()[pointI][dir];
        minCoord = min(minCoord, coord);
        maxCoord = max(maxCoord, coord);
      }
      zoneCoordinates[2*i] = returnReduce(minCoord, minOp<scalar>());
      zoneCoordinates[2*i+1] = returnReduce(maxCoord, maxOp<scalar>());
      if (debug) {
        Pout << "direction " << dir << " : "
          << "zone " << zoneName
          << " ranges from coordinate " << zoneCoordinates[2*i]
          << " to " << zoneCoordinates[2*i+1]
          << endl;
      }
    }
    zoneCoordinates.sort();
    // Slightly tweak min and max face zone so points sort within
    zoneCoordinates[0] -= SMALL;
    zoneCoordinates.last() += SMALL;
    // Check if we have static min and max mesh bounds
    const scalarField meshCoords{points0().component(dir)};
    scalar minCoord = gMin(meshCoords);
    scalar maxCoord = gMax(meshCoords);
    if (debug) {
      Pout << "direction " << dir << " : "
        << "mesh ranges from coordinate " << minCoord << " to "
        << maxCoord << endl;
    }
    // Make copy of zoneCoordinates; include min and max of mesh
    // if necessary. Mark min and max with zoneI=-1.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    labelList& rangeZone = rangeToZone_[dir];
    labelListList& rangePoints = rangeToPoints_[dir];
    List<scalarField>& rangeWeights = rangeToWeights_[dir];
    scalarField rangeToCoord{zoneCoordinates.size()};
    rangeZone.setSize(zoneCoordinates.size());
    label rangeI = 0;
    if (minCoord < zoneCoordinates[0]) {
      label sz = rangeZone.size();
      rangeToCoord.setSize(sz+1);
      rangeZone.setSize(sz+1);
      rangeToCoord[rangeI] = minCoord-SMALL;
      rangeZone[rangeI] = -1;
      if (debug) {
        Pout << "direction " << dir << " : "
          << "range " << rangeI << " at coordinate "
          << rangeToCoord[rangeI] << " from min of mesh "
          << rangeZone[rangeI] << endl;
      }
      rangeI = 1;
    }
    FOR_ALL(zoneCoordinates, i) {
      rangeToCoord[rangeI] = zoneCoordinates[i];
      rangeZone[rangeI] = zoneCoordinates.indices()[i]/2;
      if (debug) {
        Pout << "direction " << dir << " : "
          << "range " << rangeI << " at coordinate "
          << rangeToCoord[rangeI]
          << " from zone " << rangeZone[rangeI] << endl;
      }
      rangeI++;
    }
    if (maxCoord > zoneCoordinates.last()) {
      label sz = rangeToCoord.size();
      rangeToCoord.setSize(sz+1);
      rangeZone.setSize(sz+1);
      rangeToCoord[sz] = maxCoord+SMALL;
      rangeZone[sz] = -1;
      if (debug) {
        Pout << "direction " << dir << " : "
          << "range " << rangeI << " at coordinate "
          << rangeToCoord[sz] << " from max of mesh "
          << rangeZone[sz] << endl;
      }
    }
    // Sort the points
    // ~~~~~~~~~~~~~~~
    // Count all the points inbetween rangeI and rangeI+1
    labelList nRangePoints{rangeToCoord.size(), 0};
    FOR_ALL(meshCoords, pointI) {
      label rangeI = findLower(rangeToCoord, meshCoords[pointI]);
      if (rangeI == -1 || rangeI == rangeToCoord.size()-1) {
        FATAL_ERROR_IN
        (
          "displacementInterpolationMotionSolver::"
          "displacementInterpolationMotionSolver"
          "(const polyMesh&, Istream&)"
        )
        << "Did not find point " << points0()[pointI]
        << " coordinate " << meshCoords[pointI]
        << " in ranges " << rangeToCoord
        << abort(FatalError);
      }
      nRangePoints[rangeI]++;
    }
    if (debug) {
      for (label rangeI = 0; rangeI < rangeToCoord.size()-1; rangeI++) {
        // Get the two zones bounding the range
        Pout << "direction " << dir << " : "
          << "range from " << rangeToCoord[rangeI]
          << " to " << rangeToCoord[rangeI+1]
          << " contains " << nRangePoints[rangeI]
          << " points." << endl;
      }
    }
    // Sort
    rangePoints.setSize(nRangePoints.size());
    rangeWeights.setSize(nRangePoints.size());
    FOR_ALL(rangePoints, rangeI) {
      rangePoints[rangeI].setSize(nRangePoints[rangeI]);
      rangeWeights[rangeI].setSize(nRangePoints[rangeI]);
    }
    nRangePoints = 0;
    FOR_ALL(meshCoords, pointI) {
      label rangeI = findLower(rangeToCoord, meshCoords[pointI]);
      label& nPoints = nRangePoints[rangeI];
      rangePoints[rangeI][nPoints] = pointI;
      rangeWeights[rangeI][nPoints] =
        (meshCoords[pointI] - rangeToCoord[rangeI])
        /(rangeToCoord[rangeI + 1] - rangeToCoord[rangeI]);
      nPoints++;
    }
  }
}


// Destructor 
mousse::displacementInterpolationMotionSolver::
~displacementInterpolationMotionSolver()
{}


// Member Functions 
mousse::tmp<mousse::pointField>
mousse::displacementInterpolationMotionSolver::curPoints() const
{
  if (mesh().nPoints() != points0().size()) {
    FATAL_ERROR_IN
    (
      "displacementInterpolationMotionSolver::curPoints() const"
    )
    << "The number of points in the mesh seems to have changed." << endl
    << "In constant/polyMesh there are " << points0().size()
    << " points; in the current mesh there are " << mesh().nPoints()
    << " points." << exit(FatalError);
  }
  tmp<pointField> tcurPoints{new pointField{points0()}};
  pointField& curPoints = tcurPoints();
  // Interpolate the displacement of the face zones.
  vectorField zoneDisp{displacements_.size(), vector::zero};
  FOR_ALL(zoneDisp, zoneI) {
    if (times_[zoneI].size()) {
      zoneDisp[zoneI] =
        interpolateXY
        (
          mesh().time().value(),
          times_[zoneI],
          displacements_[zoneI]
        );
    }
  }
  if (debug) {
    Pout << "Zone displacements:" << zoneDisp << endl;
  }
  // Interpolate the point location
  for (direction dir = 0; dir < vector::nComponents; dir++) {
    const labelList& rangeZone = rangeToZone_[dir];
    const labelListList& rangePoints = rangeToPoints_[dir];
    const List<scalarField>& rangeWeights = rangeToWeights_[dir];
    for (label rangeI = 0; rangeI < rangeZone.size()-1; rangeI++) {
      const labelList& rPoints = rangePoints[rangeI];
      const scalarField& rWeights = rangeWeights[rangeI];
      // Get the two zones bounding the range
      label minZoneI = rangeZone[rangeI];
      //vector minDisp =
      //    (minZoneI == -1 ? vector::zero : zoneDisp[minZoneI]);
      scalar minDisp = (minZoneI == -1 ? 0.0 : zoneDisp[minZoneI][dir]);
      label maxZoneI = rangeZone[rangeI+1];
      scalar maxDisp = (maxZoneI == -1 ? 0.0 : zoneDisp[maxZoneI][dir]);
      FOR_ALL(rPoints, i) {
        label pointI = rPoints[i];
        scalar w = rWeights[i];
        curPoints[pointI][dir] += (1.0-w)*minDisp+w*maxDisp;
      }
    }
  }
  return tcurPoints;
}

