// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"
#include "map_poly_mesh.hpp"
#include "boundary_cutter.hpp"
#include "cell_splitter.hpp"
#include "edge_collapser.hpp"
#include "mesh_tools.hpp"
#include "pair.hpp"
#include "global_index.hpp"


using namespace mousse;


// Locate point on patch. Returns (mesh) point label.
label findPoint(const primitivePatch& pp, const point& nearPoint)
{
  const pointField& points = pp.points();
  const labelList& meshPoints = pp.meshPoints();
  // Find nearest and next nearest
  scalar minDistSqr = GREAT;
  label minI = -1;
  scalar almostMinDistSqr = GREAT;
  label almostMinI = -1;
  FOR_ALL(meshPoints, i) {
    label pointI = meshPoints[i];
    scalar distSqr = magSqr(nearPoint - points[pointI]);
    if (distSqr < minDistSqr) {
      almostMinDistSqr = minDistSqr;
      almostMinI = minI;
      minDistSqr = distSqr;
      minI = pointI;
    } else if (distSqr < almostMinDistSqr) {
      almostMinDistSqr = distSqr;
      almostMinI = pointI;
    }
  }
  // Decide if nearPoint unique enough.
  Info << "Found to point " << nearPoint << nl
    << "    nearest point      : " << minI
    << " distance " <<  mousse::sqrt(minDistSqr)
    << " at " << points[minI] << nl
    << "    next nearest point : " << almostMinI
    << " distance " <<  mousse::sqrt(almostMinDistSqr)
    << " at " << points[almostMinI] << endl;
  if (almostMinDistSqr < 4*minDistSqr) {
    Info << "Next nearest too close to nearest. Aborting" << endl;
    return -1;
  } else {
    return minI;
  }
}


// Locate edge on patch. Return mesh edge label.
label findEdge
(
  const primitiveMesh& mesh,
  const primitivePatch& pp,
  const point& nearPoint
)
{
  const pointField& localPoints = pp.localPoints();
  const pointField& points = pp.points();
  const labelList& meshPoints = pp.meshPoints();
  const edgeList& edges = pp.edges();
  // Find nearest and next nearest
  scalar minDist = GREAT;
  label minI = -1;
  scalar almostMinDist = GREAT;
  label almostMinI = -1;
  FOR_ALL(edges, edgeI) {
    const edge& e = edges[edgeI];
    pointHit pHit{e.line(localPoints).nearestDist(nearPoint)};
    if (!pHit.hit())
      continue;
    if (pHit.distance() < minDist) {
      almostMinDist = minDist;
      almostMinI = minI;
      minDist = pHit.distance();
      minI =
        meshTools::findEdge
        (
          mesh,
          meshPoints[e[0]],
          meshPoints[e[1]]
        );
    } else if (pHit.distance() < almostMinDist) {
      almostMinDist = pHit.distance();
      almostMinI =
        meshTools::findEdge
        (
          mesh,
          meshPoints[e[0]],
          meshPoints[e[1]]
        );
    }
  }
  if (minI == -1) {
    Info << "Did not find edge close to point " << nearPoint << endl;
    return -1;
  }
  // Decide if nearPoint unique enough.
  Info << "Found to point " << nearPoint << nl
    << "    nearest edge      : " << minI
    << " distance " << minDist << " endpoints "
    << mesh.edges()[minI].line(points) << nl
    << "    next nearest edge : " << almostMinI
    << " distance " << almostMinDist << " endpoints "
    << mesh.edges()[almostMinI].line(points) << nl
    << endl;
  if (almostMinDist < 2*minDist) {
    Info << "Next nearest too close to nearest. Aborting" << endl;
    return -1;
  } else {
    return minI;
  }
}


// Find face on patch. Return mesh face label.
label findFace
(
  const primitiveMesh& mesh,
  const primitivePatch& pp,
  const point& nearPoint
)
{
  const pointField& points = pp.points();
  // Find nearest and next nearest
  scalar minDist = GREAT;
  label minI = -1;
  scalar almostMinDist = GREAT;
  label almostMinI = -1;
  FOR_ALL(pp, patchFaceI) {
    pointHit pHit(pp[patchFaceI].nearestPoint(nearPoint, points));
    if (!pHit.hit())
      continue;
    if (pHit.distance() < minDist) {
      almostMinDist = minDist;
      almostMinI = minI;
      minDist = pHit.distance();
      minI = patchFaceI + mesh.nInternalFaces();
    } else if (pHit.distance() < almostMinDist) {
      almostMinDist = pHit.distance();
      almostMinI = patchFaceI + mesh.nInternalFaces();
    }
  }
  if (minI == -1) {
    Info << "Did not find face close to point " << nearPoint << endl;
    return -1;
  }
  // Decide if nearPoint unique enough.
  Info << "Found to point " << nearPoint << nl
    << "    nearest face      : " << minI
    << " distance " << minDist
    << " to face centre " << mesh.faceCentres()[minI] << nl
    << "    next nearest face : " << almostMinI
    << " distance " << almostMinDist
    << " to face centre " << mesh.faceCentres()[almostMinI] << nl
    << endl;
  if (almostMinDist < 2*minDist) {
    Info << "Next nearest too close to nearest. Aborting" << endl;
    return -1;
  } else {
    return minI;
  }
}


// Find cell with cell centre close to given point.
label findCell(const primitiveMesh& mesh, const point& nearPoint)
{
  label cellI = mesh.findCell(nearPoint);
  if (cellI != -1) {
    scalar distToCcSqr = magSqr(nearPoint - mesh.cellCentres()[cellI]);
    const labelList& cPoints = mesh.cellPoints()[cellI];
    label minI = -1;
    scalar minDistSqr = GREAT;
    FOR_ALL(cPoints, i) {
      label pointI = cPoints[i];
      scalar distSqr = magSqr(nearPoint - mesh.points()[pointI]);
      if (distSqr < minDistSqr) {
        minDistSqr = distSqr;
        minI = pointI;
      }
    }
    // Decide if nearPoint unique enough.
    Info << "Found to point " << nearPoint << nl
      << "    nearest cell       : " << cellI
      << " distance " << mousse::sqrt(distToCcSqr)
      << " to cell centre " << mesh.cellCentres()[cellI] << nl
      << "    nearest mesh point : " << minI
      << " distance " << mousse::sqrt(minDistSqr)
      << " to " << mesh.points()[minI] << nl
      << endl;
    if (minDistSqr < 4*distToCcSqr) {
      Info << "Mesh point too close to nearest cell centre. Aborting"
        << endl;
      cellI = -1;
    }
  }
  return cellI;
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const bool overwrite = args.optionFound("overwrite");
  Info << "Reading modifyMeshDict\n" << endl;
  IOdictionary dict
  {
    {
      "modifyMeshDict",
      runTime.system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  // Read all from the dictionary.
  List<Pair<point>> pointsToMove{dict.lookup("pointsToMove")};
  List<Pair<point>> edgesToSplit{dict.lookup("edgesToSplit")};
  List<Pair<point>> facesToTriangulate{dict.lookup("facesToTriangulate")};
  bool cutBoundary =
    pointsToMove.size() || edgesToSplit.size() || facesToTriangulate.size();
  List<Pair<point>> edgesToCollapse{dict.lookup("edgesToCollapse")};
  bool collapseEdge = edgesToCollapse.size();
  List<Pair<point>> cellsToPyramidise{dict.lookup("cellsToSplit")};
  bool cellsToSplit = cellsToPyramidise.size();
  Info << "Read from " << dict.name() << nl
    << "  Boundary cutting module:" << nl
    << "    points to move      :" << pointsToMove.size() << nl
    << "    edges to split      :" << edgesToSplit.size() << nl
    << "    faces to triangulate:" << facesToTriangulate.size() << nl
    << "  Cell splitting module:" << nl
    << "    cells to split      :" << cellsToPyramidise.size() << nl
    << "  Edge collapsing module:" << nl
    << "    edges to collapse   :" << edgesToCollapse.size() << nl
    << endl;
  if ((cutBoundary && collapseEdge)
      || (cutBoundary && cellsToSplit)
      || (collapseEdge && cellsToSplit)) {
    FATAL_ERROR_IN(args.executable())
      << "Used more than one mesh modifying module "
      << "(boundary cutting, cell splitting, edge collapsing)" << nl
      << "Please do them in separate passes." << exit(FatalError);
  }
  // Get calculating engine for all of outside
  const SubList<face> outsideFaces
  {
    mesh.faces(),
    mesh.nFaces() - mesh.nInternalFaces(),
    mesh.nInternalFaces()
  };
  primitivePatch allBoundary{outsideFaces, mesh.points()};
  // Look up mesh labels and convert to input for boundaryCutter.
  bool validInputs = true;
  Info << nl << "Looking up points to move ..." << nl << endl;
  Map<point> pointToPos{pointsToMove.size()};
  FOR_ALL(pointsToMove, i) {
    const Pair<point>& pts = pointsToMove[i];
    label pointI = findPoint(allBoundary, pts.first());
    if (pointI == -1 || !pointToPos.insert(pointI, pts.second())) {
      Info << "Could not insert mesh point " << pointI
        << " for input point " << pts.first() << nl
        << "Perhaps the point is already marked for moving?" << endl;
      validInputs = false;
    }
  }
  Info << nl << "Looking up edges to split ..." << nl << endl;
  Map<List<point>> edgeToCuts{edgesToSplit.size()};
  FOR_ALL(edgesToSplit, i) {
    const Pair<point>& pts = edgesToSplit[i];
    label edgeI = findEdge(mesh, allBoundary, pts.first());
    if (edgeI == -1
        || !edgeToCuts.insert(edgeI, List<point>(1, pts.second()))) {
      Info << "Could not insert mesh edge " << edgeI
        << " for input point " << pts.first() << nl
        << "Perhaps the edge is already marked for cutting?" << endl;
      validInputs = false;
    }
  }
  Info << nl << "Looking up faces to triangulate ..." << nl << endl;
  Map<point> faceToDecompose{facesToTriangulate.size()};
  FOR_ALL(facesToTriangulate, i) {
    const Pair<point>& pts = facesToTriangulate[i];
    label faceI = findFace(mesh, allBoundary, pts.first());
    if (faceI == -1 || !faceToDecompose.insert(faceI, pts.second())) {
      Info << "Could not insert mesh face " << faceI
        << " for input point " << pts.first() << nl
        << "Perhaps the face is already marked for splitting?" << endl;
      validInputs = false;
    }
  }
  Info << nl << "Looking up cells to convert to pyramids around"
    << " cell centre ..." << nl << endl;
  Map<point> cellToPyrCentre{cellsToPyramidise.size()};
  FOR_ALL(cellsToPyramidise, i) {
    const Pair<point>& pts = cellsToPyramidise[i];
    label cellI = findCell(mesh, pts.first());
    if (cellI == -1 || !cellToPyrCentre.insert(cellI, pts.second())) {
      Info << "Could not insert mesh cell " << cellI
        << " for input point " << pts.first() << nl
        << "Perhaps the cell is already marked for splitting?" << endl;
      validInputs = false;
    }
  }
  Info << nl << "Looking up edges to collapse ..." << nl << endl;
  Map<point> edgeToPos{edgesToCollapse.size()};
  FOR_ALL(edgesToCollapse, i) {
    const Pair<point>& pts = edgesToCollapse[i];
    label edgeI = findEdge(mesh, allBoundary, pts.first());
    if (edgeI == -1 || !edgeToPos.insert(edgeI, pts.second())) {
      Info << "Could not insert mesh edge " << edgeI
        << " for input point " << pts.first() << nl
        << "Perhaps the edge is already marked for collaping?" << endl;
      validInputs = false;
    }
  }
  if (!validInputs) {
    Info << nl << "There was a problem in one of the inputs in the"
      << " dictionary. Not modifying mesh." << endl;
  } else if (cellToPyrCentre.size()) {
    Info << nl << "All input cells located. Modifying mesh." << endl;
    // Mesh change engine
    cellSplitter cutter{mesh};
    // Topo change container
    polyTopoChange meshMod{mesh};
    // Insert commands into meshMod
    cutter.setRefinement(cellToPyrCentre, meshMod);
    // Do changes
    autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(mesh, false);
    if (morphMap().hasMotionPoints()) {
      mesh.movePoints(morphMap().preMotionPoints());
    }
    cutter.updateMesh(morphMap());
    if (!overwrite) {
      runTime++;
    } else {
      mesh.setInstance(oldInstance);
    }
    // Write resulting mesh
    Info << "Writing modified mesh to time " << runTime.timeName() << endl;
    mesh.write();
  } else if (edgeToPos.size()) {
    Info << nl << "All input edges located. Modifying mesh." << endl;
    // Mesh change engine
    edgeCollapser cutter{mesh};
    const edgeList& edges = mesh.edges();
    const pointField& points = mesh.points();
    pointField newPoints{points};
    PackedBoolList collapseEdge{mesh.nEdges()};
    Map<point> collapsePointToLocation{mesh.nPoints()};
    // Get new positions and construct collapse network
    FOR_ALL_CONST_ITER(Map<point>, edgeToPos, iter) {
      label edgeI = iter.key();
      const edge& e = edges[edgeI];
      collapseEdge[edgeI] = true;
      collapsePointToLocation.set(e[1], points[e[0]]);
      newPoints[e[0]] = iter();
    }
    // Move master point to destination.
    mesh.movePoints(newPoints);
    List<pointEdgeCollapse> allPointInfo;
    const globalIndex globalPoints{mesh.nPoints()};
    labelList pointPriority{mesh.nPoints(), 0};
    cutter.consistentCollapse
      (
        globalPoints,
        pointPriority,
        collapsePointToLocation,
        collapseEdge,
        allPointInfo
      );
    // Topo change container
    polyTopoChange meshMod{mesh};
    // Insert
    cutter.setRefinement(allPointInfo, meshMod);
    // Do changes
    autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(mesh, false);
    if (morphMap().hasMotionPoints()) {
      mesh.movePoints(morphMap().preMotionPoints());
    }
    // Not implemented yet:
    //cutter.updateMesh(morphMap());
    if (!overwrite) {
      runTime++;
    } else {
      mesh.setInstance(oldInstance);
    }
    // Write resulting mesh
    Info << "Writing modified mesh to time " << runTime.timeName() << endl;
    mesh.write();
  } else {
    Info << nl << "All input points located. Modifying mesh." << endl;
    // Mesh change engine
    boundaryCutter cutter{mesh};
    // Topo change container
    polyTopoChange meshMod{mesh};
    // Insert commands into meshMod
    cutter.setRefinement
      (
        pointToPos,
        edgeToCuts,
        Map<labelPair>{0},  // Faces to split diagonally
        faceToDecompose,    // Faces to triangulate
        meshMod
      );
    // Do changes
    autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(mesh, false);
    if (morphMap().hasMotionPoints()) {
      mesh.movePoints(morphMap().preMotionPoints());
    }
    cutter.updateMesh(morphMap());
    if (!overwrite) {
      runTime++;
    } else {
      mesh.setInstance(oldInstance);
    }
    // Write resulting mesh
    Info << "Writing modified mesh to time " << runTime.timeName() << endl;
    mesh.write();
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

