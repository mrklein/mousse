// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_classification.hpp"
#include "tri_surface_search.hpp"
#include "indexed_octree.hpp"
#include "tree_data_face.hpp"
#include "mesh_search.hpp"
#include "cell_info.hpp"
#include "poly_mesh.hpp"
#include "mesh_wave.hpp"
#include "list_ops.hpp"
#include "mesh_tools.hpp"
#include "cpu_time.hpp"
#include "tri_surface.hpp"
#include "global_mesh_data.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cellClassification, 0);

}


// Private Member Functions
mousse::label mousse::cellClassification::count
(
  const labelList& elems,
  const label elem
)
{
  label cnt = 0;
  FOR_ALL(elems, elemI) {
    if (elems[elemI] == elem) {
      cnt++;
    }
  }
  return cnt;
}


// Mark all faces that are cut by the surface. Two pass:
// Pass1: mark all mesh edges that intersect surface (quick since triangle
// pierce test).
// Pass2: Check for all point neighbours of these faces whether any of their
// faces are pierced.
mousse::boolList mousse::cellClassification::markFaces
(
  const triSurfaceSearch& search
) const
{
  cpuTime timer;
  boolList cutFace(mesh_.nFaces(), false);
  label nCutFaces = 0;
  // Intersect mesh edges with surface (is fast) and mark all faces that
  // use edge.
  FOR_ALL(mesh_.edges(), edgeI) {
    if (debug && (edgeI % 10000 == 0)) {
      Pout << "Intersecting mesh edge " << edgeI << " with surface"
        << endl;
    }
    const edge& e = mesh_.edges()[edgeI];
    const point& p0 = mesh_.points()[e.start()];
    const point& p1 = mesh_.points()[e.end()];
    pointIndexHit pHit(search.tree().findLineAny(p0, p1));
    if (pHit.hit()) {
      const labelList& myFaces = mesh_.edgeFaces()[edgeI];
      FOR_ALL(myFaces, myFaceI) {
        label faceI = myFaces[myFaceI];
        if (!cutFace[faceI]) {
          cutFace[faceI] = true;
          nCutFaces++;
        }
      }
    }
  }
  if (debug) {
    Pout << "Intersected edges of mesh with surface in = "
      << timer.cpuTimeIncrement() << " s\n" << endl << endl;
  }
  //
  // Construct octree on faces that have not yet been marked as cut
  //
  labelList allFaces{mesh_.nFaces() - nCutFaces};
  label allFaceI = 0;
  FOR_ALL(cutFace, faceI) {
    if (!cutFace[faceI]) {
      allFaces[allFaceI++] = faceI;
    }
  }
  if (debug) {
    Pout << "Testing " << allFaceI << " faces for piercing by surface"
      << endl;
  }
  treeBoundBox allBb{mesh_.points()};
  // Extend domain slightly (also makes it 3D if was 2D)
  scalar tol = 1e-6*allBb.avgDim();
  point& bbMin = allBb.min();
  bbMin.x() -= tol;
  bbMin.y() -= tol;
  bbMin.z() -= tol;
  point& bbMax = allBb.max();
  bbMax.x() += 2*tol;
  bbMax.y() += 2*tol;
  bbMax.z() += 2*tol;
  indexedOctree<treeDataFace> faceTree
  {
    treeDataFace(false, mesh_, allFaces),
    allBb,      // overall search domain
    8,          // maxLevel
    10,         // leafsize
    3.0         // duplicity
  };
  const triSurface& surf = search.surface();
  const edgeList& edges = surf.edges();
  const pointField& localPoints = surf.localPoints();
  label nAddFaces = 0;
  FOR_ALL(edges, edgeI) {
    if (debug && (edgeI % 10000 == 0)) {
      Pout << "Intersecting surface edge " << edgeI
        << " with mesh faces" << endl;
    }
    const edge& e = edges[edgeI];
    const point& start = localPoints[e.start()];
    const point& end = localPoints[e.end()];
    vector edgeNormal(end - start);
    const scalar edgeMag = mag(edgeNormal);
    const vector smallVec = 1e-9*edgeNormal;
    edgeNormal /= edgeMag+VSMALL;
    // Current start of pierce test
    point pt = start;
    while (true) {
      pointIndexHit pHit(faceTree.findLine(pt, end));
      if (!pHit.hit()) {
        break;
      } else {
        label faceI = faceTree.shapes().faceLabels()[pHit.index()];
        if (!cutFace[faceI]) {
          cutFace[faceI] = true;
          nAddFaces++;
        }
        // Restart from previous endpoint
        pt = pHit.hitPoint() + smallVec;
        if (((pt-start) & edgeNormal) >= edgeMag) {
          break;
        }
      }
    }
  }
  if (debug) {
    Pout << "Detected an additional " << nAddFaces << " faces cut"
      << endl;
    Pout << "Intersected edges of surface with mesh faces in = "
      << timer.cpuTimeIncrement() << " s\n" << endl << endl;
  }
  return cutFace;
}


// Determine faces cut by surface and use to divide cells into types. See
// cellInfo. All cells reachable from outsidePts are considered to be of type
// 'outside'
void mousse::cellClassification::markCells
(
  const meshSearch& queryMesh,
  const boolList& piercedFace,
  const pointField& outsidePts
)
{
  // Use meshwave to partition mesh, starting from outsidePt
  // Construct null; sets type to NOTSET
  List<cellInfo> cellInfoList{mesh_.nCells()};
  // Mark cut cells first
  FOR_ALL(piercedFace, faceI) {
    if (piercedFace[faceI]) {
      cellInfoList[mesh_.faceOwner()[faceI]] =
        cellInfo(cellClassification::CUT);
      if (mesh_.isInternalFace(faceI)) {
        cellInfoList[mesh_.faceNeighbour()[faceI]] =
          cellInfo(cellClassification::CUT);
      }
    }
  }
  //
  // Mark cells containing outside points as being outside
  //
  // Coarse guess number of faces
  labelHashSet outsideFacesMap{outsidePts.size()*6*2};
  FOR_ALL(outsidePts, outsidePtI) {
    // Use linear search for points.
    label cellI = queryMesh.findCell(outsidePts[outsidePtI], -1, false);
    if (returnReduce(cellI, maxOp<label>()) == -1) {
      FATAL_ERROR_IN
      (
        "List<cellClassification::cType> markCells"
        "(const meshSearch&, const boolList&, const pointField&)"
      )
      << "outsidePoint " << outsidePts[outsidePtI]
      << " is not inside any cell"
      << nl << "It might be on a face or outside the geometry"
      << exit(FatalError);
    }
    if (cellI >= 0) {
      cellInfoList[cellI] = cellInfo(cellClassification::OUTSIDE);
      // Mark faces of cellI
      const labelList& myFaces = mesh_.cells()[cellI];
      FOR_ALL(myFaces, myFaceI) {
        outsideFacesMap.insert(myFaces[myFaceI]);
      }
    }
  }
  //
  // Mark faces to start wave from
  //
  labelList changedFaces{outsideFacesMap.toc()};
  List<cellInfo> changedFacesInfo
  {
    changedFaces.size(),
    cellInfo(cellClassification::OUTSIDE)
  };
  MeshWave<cellInfo> cellInfoCalc
  {
    mesh_,
    changedFaces,                       // Labels of changed faces
    changedFacesInfo,                   // Information on changed faces
    cellInfoList,                       // Information on all cells
    mesh_.globalData().nTotalCells()+1  // max iterations
  };
  // Get information out of cellInfoList
  const List<cellInfo>& allInfo = cellInfoCalc.allCellInfo();
  FOR_ALL(allInfo, cellI) {
    label t = allInfo[cellI].type();
    if (t == cellClassification::NOTSET) {
      t = cellClassification::INSIDE;
    }
    operator[](cellI) = t;
  }
}


void mousse::cellClassification::classifyPoints
(
  const label meshType,
  const labelList& cellType,
  List<pointStatus>& pointSide
) const
{
  pointSide.setSize(mesh_.nPoints());
  FOR_ALL(mesh_.pointCells(), pointI) {
    const labelList& pCells = mesh_.pointCells()[pointI];
    pointSide[pointI] = UNSET;
    FOR_ALL(pCells, i) {
      label type = cellType[pCells[i]];
      if (type == meshType) {
        if (pointSide[pointI] == UNSET) {
          pointSide[pointI] = MESH;
        } else if (pointSide[pointI] == NONMESH) {
          pointSide[pointI] = MIXED;
          break;
        }
      } else {
        if (pointSide[pointI] == UNSET) {
          pointSide[pointI] = NONMESH;
        } else if (pointSide[pointI] == MESH) {
          pointSide[pointI] = MIXED;
          break;
        }
      }
    }
  }
}


bool mousse::cellClassification::usesMixedPointsOnly
(
  const List<pointStatus>& pointSide,
  const label cellI
) const
{
  const faceList& faces = mesh_.faces();
  const cell& cFaces = mesh_.cells()[cellI];
  FOR_ALL(cFaces, cFaceI) {
    const face& f = faces[cFaces[cFaceI]];
    FOR_ALL(f, fp) {
      if (pointSide[f[fp]] != MIXED) {
        return false;
      }
    }
  }
  // All points are mixed.
  return true;
}


void mousse::cellClassification::getMeshOutside
(
  const label meshType,
  faceList& outsideFaces,
  labelList& outsideOwner
) const
{
  const labelList& own = mesh_.faceOwner();
  const labelList& nbr = mesh_.faceNeighbour();
  const faceList& faces = mesh_.faces();
  outsideFaces.setSize(mesh_.nFaces());
  outsideOwner.setSize(mesh_.nFaces());
  label outsideI = 0;
  // Get faces on interface between meshType and non-meshType
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++) {
    label ownType = operator[](own[faceI]);
    label nbrType = operator[](nbr[faceI]);
    if (ownType == meshType && nbrType != meshType) {
      outsideFaces[outsideI] = faces[faceI];
      outsideOwner[outsideI] = own[faceI];    // meshType cell
      outsideI++;
    } else if (ownType != meshType && nbrType == meshType) {
      outsideFaces[outsideI] = faces[faceI];
      outsideOwner[outsideI] = nbr[faceI];    // meshType cell
      outsideI++;
    }
  }
  // Get faces on outside of real mesh with cells of meshType.
  for (label faceI = mesh_.nInternalFaces(); faceI < mesh_.nFaces(); faceI++) {
    if (operator[](own[faceI]) == meshType) {
      outsideFaces[outsideI] = faces[faceI];
      outsideOwner[outsideI] = own[faceI];    // meshType cell
      outsideI++;
    }
  }
  outsideFaces.setSize(outsideI);
  outsideOwner.setSize(outsideI);
}


// Constructors

// Construct from mesh and surface and point(s) on outside
mousse::cellClassification::cellClassification
(
  const polyMesh& mesh,
  const meshSearch& meshQuery,
  const triSurfaceSearch& surfQuery,
  const pointField& outsidePoints
)
:
  labelList{mesh.nCells(), cellClassification::NOTSET},
  mesh_{mesh}
{
  markCells
  (
    meshQuery,
    markFaces(surfQuery),
    outsidePoints
  );
}


// Construct from mesh and meshType.
mousse::cellClassification::cellClassification
(
  const polyMesh& mesh,
  const labelList& cellType
)
:
  labelList{cellType},
  mesh_{mesh}
{
  if (mesh_.nCells() != size()) {
    FATAL_ERROR_IN
    (
      "cellClassification::cellClassification"
      "(const polyMesh&, const labelList&)"
    )
    << "Number of elements of cellType argument is not equal to the"
    << " number of cells" << abort(FatalError);
  }
}


// Construct as copy
mousse::cellClassification::cellClassification(const cellClassification& cType)
:
  labelList{cType},
  mesh_{cType.mesh()}
{}


// Member Functions
// Makes cutCells further than nLayers away from meshType to
// fillType. Returns number of cells changed.
mousse::label mousse::cellClassification::trimCutCells
(
  const label nLayers,
  const label meshType,
  const label fillType
)
{
  // Temporary cell type for growing.
  labelList newCellType(*this);
  newCellType = *this;
  // Do point-cell-point walk on newCellType out from meshType cells
  for (label iter = 0; iter < nLayers; iter++) {
    // Get status of points: visible from meshType (pointSide == MESH)
    // or fillType/cutCells cells (pointSide == NONMESH) or
    // both (pointSide == MIXED)
    List<pointStatus> pointSide{mesh_.nPoints()};
    classifyPoints(meshType, newCellType, pointSide);
    // Grow layer of outside cells
    FOR_ALL(pointSide, pointI) {
      if (pointSide[pointI] == MIXED) {
        // Make cut
        const labelList& pCells = mesh_.pointCells()[pointI];
        FOR_ALL(pCells, i) {
          label type = newCellType[pCells[i]];
          if (type == cellClassification::CUT) {
            // Found cut cell sharing point with
            // mesh type cell. Grow.
            newCellType[pCells[i]] = meshType;
          }
        }
      }
    }
  }
  // Merge newCellType into *this:
  // - leave meshType cells intact
  // - leave nonMesh cells intact
  // - make cutcells fillType if they were not marked in newCellType
  label nChanged = 0;
  FOR_ALL(newCellType, cellI) {
    if (operator[](cellI) == cellClassification::CUT) {
      if (newCellType[cellI] != meshType) {
        // Cell was cutCell but further than nLayers away from
        // meshType. Convert to fillType.
        operator[](cellI) = fillType;
        nChanged++;
      }
    }
  }
  return nChanged;
}


// Grow surface by pointNeighbours of type meshType
mousse::label mousse::cellClassification::growSurface
(
  const label meshType,
  const label fillType
)
{
  boolList hasMeshType(mesh_.nPoints(), false);
  // Mark points used by meshType cells
  FOR_ALL(mesh_.pointCells(), pointI) {
    const labelList& myCells = mesh_.pointCells()[pointI];
    // Check if one of cells has meshType
    FOR_ALL(myCells, myCellI) {
      label type = operator[](myCells[myCellI]);
      if (type == meshType) {
        hasMeshType[pointI] = true;
        break;
      }
    }
  }
  // Change neighbours of marked points
  label nChanged = 0;
  FOR_ALL(hasMeshType, pointI) {
    if (!hasMeshType[pointI])
      continue;
    const labelList& myCells = mesh_.pointCells()[pointI];
    FOR_ALL(myCells, myCellI) {
      if (operator[](myCells[myCellI]) != meshType) {
        operator[](myCells[myCellI]) = fillType;
        nChanged++;
      }
    }
  }
  return nChanged;
}


// Check all points used by cells of meshType for use of at least one point
// which is not on the outside. If all points are on the outside of the mesh
// this would probably result in a flattened cell when projecting the cell
// onto the surface.
mousse::label mousse::cellClassification::fillHangingCells
(
  const label meshType,
  const label fillType,
  const label maxIter
)
{
  label nTotChanged = 0;
  for (label iter = 0; iter < maxIter; iter++) {
    label nChanged = 0;
    // Get status of points: visible from meshType or non-meshType cells
    List<pointStatus> pointSide{mesh_.nPoints()};
    classifyPoints(meshType, *this, pointSide);

    // Check all cells using mixed point type for whether they use mixed
    // points only. Note: could probably speed this up by counting number
    // of mixed verts per face and mixed faces per cell or something?
    FOR_ALL(pointSide, pointI) {
      if (pointSide[pointI] != MIXED)
        continue;
      const labelList& pCells = mesh_.pointCells()[pointI];
      FOR_ALL(pCells, i) {
        label cellI = pCells[i];
        if (operator[](cellI) != meshType)
          continue;
        if (usesMixedPointsOnly(pointSide, cellI)) {
          operator[](cellI) = fillType;
          nChanged++;
        }
      }
    }
    nTotChanged += nChanged;
    Pout << "removeHangingCells : changed " << nChanged
      << " hanging cells" << endl;
    if (nChanged == 0) {
      break;
    }
  }
  return nTotChanged;
}


mousse::label mousse::cellClassification::fillRegionEdges
(
  const label meshType,
  const label fillType,
  const label maxIter
)
{
  label nTotChanged = 0;
  for (label iter = 0; iter < maxIter; iter++) {
    // Get interface between meshType cells and non-meshType cells as a list
    // of faces and for each face the cell which is the meshType.
    faceList outsideFaces;
    labelList outsideOwner;
    getMeshOutside(meshType, outsideFaces, outsideOwner);
    // Build primitivePatch out of it and check it for problems.
    primitiveFacePatch fp{outsideFaces, mesh_.points()};
    const labelListList& edgeFaces = fp.edgeFaces();
    label nChanged = 0;
    // Check all edgeFaces for non-manifoldness
    FOR_ALL(edgeFaces, edgeI) {
      const labelList& eFaces = edgeFaces[edgeI];
      if (eFaces.size() <= 2)
        continue;
      // patch connected through pinched edge. Remove first face using
      // edge (and not yet changed)
      FOR_ALL(eFaces, i) {
        label patchFaceI = eFaces[i];
        label ownerCell = outsideOwner[patchFaceI];
        if (operator[](ownerCell) == meshType) {
          operator[](ownerCell) = fillType;
          nChanged++;
          break;
        }
      }
    }
    nTotChanged += nChanged;
    Pout << "fillRegionEdges : changed " << nChanged
      << " cells using multiply connected edges" << endl;
    if (nChanged == 0) {
      break;
    }
  }
  return nTotChanged;
}


mousse::label mousse::cellClassification::fillRegionPoints
(
  const label meshType,
  const label fillType,
  const label maxIter
)
{
  label nTotChanged = 0;
  for (label iter = 0; iter < maxIter; iter++) {
    // Get interface between meshType cells and non-meshType cells as a list
    // of faces and for each face the cell which is the meshType.
    faceList outsideFaces;
    labelList outsideOwner;
    getMeshOutside(meshType, outsideFaces, outsideOwner);
    // Build primitivePatch out of it and check it for problems.
    primitiveFacePatch fp(outsideFaces, mesh_.points());
    labelHashSet nonManifoldPoints;
    // Check for non-manifold points.
    fp.checkPointManifold(false, &nonManifoldPoints);
    const Map<label>& meshPointMap = fp.meshPointMap();
    label nChanged = 0;
    FOR_ALL_CONST_ITER(labelHashSet, nonManifoldPoints, iter) {
      // Find a face on fp using point and remove it.
      const label patchPointI = meshPointMap[iter.key()];
      const labelList& pFaces = fp.pointFaces()[patchPointI];
      // Remove any face using conflicting point. Does first face which
      // has not yet been done. Could be more intelligent and decide which
      // one would be best to remove.
      FOR_ALL(pFaces, i) {
        const label patchFaceI = pFaces[i];
        const label ownerCell  = outsideOwner[patchFaceI];
        if (operator[](ownerCell) == meshType) {
          operator[](ownerCell) = fillType;
          nChanged++;
          break;
        }
      }
    }
    nTotChanged += nChanged;
    Pout << "fillRegionPoints : changed " << nChanged
      << " cells using multiply connected points" << endl;
    if (nChanged == 0) {
      break;
    }
  }
  return nTotChanged;
}


void mousse::cellClassification::writeStats(Ostream& os) const
{
  os << "Cells:" << size() << endl
    << "    notset  : " << count(*this, NOTSET) << endl
    << "    cut     : " << count(*this, CUT) << endl
    << "    inside  : " << count(*this, INSIDE) << endl
    << "    outside : " << count(*this, OUTSIDE) << endl;
}

// Member Operators
void mousse::cellClassification::operator=(const mousse::cellClassification& rhs)
{
  labelList::operator=(rhs);
}

