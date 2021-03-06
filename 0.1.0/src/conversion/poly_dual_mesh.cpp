// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_dual_mesh.hpp"
#include "mesh_tools.hpp"
#include "ofstream.hpp"
#include "time.hpp"
#include "sortable_list.hpp"
#include "point_set.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(polyDualMesh, 0);

}

// Private Member Functions 
// Determine order for faces:
// - upper-triangular order for internal faces
// - external faces after internal faces (were already so)
mousse::labelList mousse::polyDualMesh::getFaceOrder
(
  const labelList& faceOwner,
  const labelList& faceNeighbour,
  const cellList& cells,
  label& nInternalFaces
)
{
  labelList oldToNew{faceOwner.size(), -1};
  // First unassigned face
  label newFaceI = 0;
  FOR_ALL(cells, cellI) {
    const labelList& cFaces = cells[cellI];
    SortableList<label> nbr{cFaces.size()};
    FOR_ALL(cFaces, i) {
      label faceI = cFaces[i];
      label nbrCellI = faceNeighbour[faceI];
      if (nbrCellI != -1) {
        // Internal face. Get cell on other side.
        if (nbrCellI == cellI) {
          nbrCellI = faceOwner[faceI];
        }
        if (cellI < nbrCellI) {
          // CellI is master
          nbr[i] = nbrCellI;
        } else {
          // nbrCell is master. Let it handle this face.
          nbr[i] = -1;
        }
      } else {
        // External face. Do later.
        nbr[i] = -1;
      }
    }
    nbr.sort();
    FOR_ALL(nbr, i) {
      if (nbr[i] != -1) {
        oldToNew[cFaces[nbr.indices()[i]]] = newFaceI++;
      }
    }
  }
  nInternalFaces = newFaceI;
  Pout << "nInternalFaces:" << nInternalFaces << endl;
  Pout << "nFaces:" << faceOwner.size() << endl;
  Pout << "nCells:" << cells.size() << endl;
  // Leave patch faces intact.
  for (label faceI = newFaceI; faceI < faceOwner.size(); faceI++) {
    oldToNew[faceI] = faceI;
  }
  // Check done all faces.
  FOR_ALL(oldToNew, faceI) {
    if (oldToNew[faceI] == -1) {
      FATAL_ERROR_IN
      (
        "polyDualMesh::getFaceOrder"
        "(const labelList&, const labelList&, const label) const"
      )
      << "Did not determine new position"
      << " for face " << faceI
      << abort(FatalError);
    }
  }
  return oldToNew;
}


// Get the two edges on faceI using pointI. Returns them such that the order
// - otherVertex of e0
// - pointI
// - otherVertex(pointI) of e1
// is in face order
void mousse::polyDualMesh::getPointEdges
(
  const primitivePatch& patch,
  const label faceI,
  const label pointI,
  label& e0,
  label& e1
)
{
  const labelList& fEdges = patch.faceEdges()[faceI];
  const face& f = patch.localFaces()[faceI];
  e0 = -1;
  e1 = -1;
  FOR_ALL(fEdges, i) {
    label edgeI = fEdges[i];
    const edge& e = patch.edges()[edgeI];
    if (e[0] == pointI) {
      // One of the edges using pointI. Check which one.
      label index = findIndex(f, pointI);
      if (f.nextLabel(index) == e[1]) {
        e1 = edgeI;
      } else {
        e0 = edgeI;
      }
      if (e0 != -1 && e1 != -1) {
        return;
      }
    } else if (e[1] == pointI) {
      // One of the edges using pointI. Check which one.
      label index = findIndex(f, pointI);
      if (f.nextLabel(index) == e[0]) {
        e1 = edgeI;
      } else {
        e0 = edgeI;
      }
      if (e0 != -1 && e1 != -1) {
        return;
      }
    }
  }
  FATAL_ERROR_IN("getPointEdges") << "Cannot find two edges on face:" << faceI
    << " vertices:" << patch.localFaces()[faceI]
    << " that uses point:" << pointI
    << abort(FatalError);
}


// Collect the face on an external point of the patch.
mousse::labelList mousse::polyDualMesh::collectPatchSideFace
(
  const polyPatch& patch,
  const label patchToDualOffset,
  const labelList& edgeToDualPoint,
  const labelList& pointToDualPoint,
  const label pointI,
  label& edgeI
)
{
  // Construct face by walking around e[eI] starting from
  // patchEdgeI
  label meshPointI = patch.meshPoints()[pointI];
  const labelList& pFaces = patch.pointFaces()[pointI];
  DynamicList<label> dualFace;
  if (pointToDualPoint[meshPointI] >= 0) {
    // Number of pFaces + 2 boundary edge + feature point
    dualFace.setCapacity(pFaces.size()+2+1);
    // Store dualVertex for feature edge
    dualFace.append(pointToDualPoint[meshPointI]);
  } else {
    dualFace.setCapacity(pFaces.size()+2);
  }
  // Store dual vertex for starting edge.
  if (edgeToDualPoint[patch.meshEdges()[edgeI]] < 0) {
    FATAL_ERROR_IN("polyDualMesh::collectPatchSideFace") << edgeI
      << abort(FatalError);
  }
  dualFace.append(edgeToDualPoint[patch.meshEdges()[edgeI]]);
  label faceI = patch.edgeFaces()[edgeI][0];
  // Check order of vertices of edgeI in face to see if we need to reverse.
  bool reverseFace;
  label e0, e1;
  getPointEdges(patch, faceI, pointI, e0, e1);
  if (e0 == edgeI) {
    reverseFace = true;
  } else {
    reverseFace = false;
  }
  while (true) {
    // Store dual vertex for faceI.
    dualFace.append(faceI + patchToDualOffset);
    // Cross face to other edge on pointI
    label e0, e1;
    getPointEdges(patch, faceI, pointI, e0, e1);
    if (e0 == edgeI) {
      edgeI = e1;
    } else {
      edgeI = e0;
    }
    if (edgeToDualPoint[patch.meshEdges()[edgeI]] >= 0) {
      // Feature edge. Insert dual vertex for edge.
      dualFace.append(edgeToDualPoint[patch.meshEdges()[edgeI]]);
    }
    const labelList& eFaces = patch.edgeFaces()[edgeI];
    if (eFaces.size() == 1) {
      // Reached other edge of patch
      break;
    }
    // Cross edge to other face.
    if (eFaces[0] == faceI) {
      faceI = eFaces[1];
    } else {
      faceI = eFaces[0];
    }
  }
  dualFace.shrink();
  if (reverseFace) {
    reverse(dualFace);
  }
  return dualFace;
}


// Collect face around pointI which is not on the outside of the patch.
// Returns the vertices of the face and the indices in these vertices of
// any points which are on feature edges.
void mousse::polyDualMesh::collectPatchInternalFace
(
  const polyPatch& patch,
  const label patchToDualOffset,
  const labelList& edgeToDualPoint,
  const label pointI,
  const label startEdgeI,
  labelList& dualFace2,
  labelList& featEdgeIndices2
)
{
  // Construct face by walking around pointI starting from startEdgeI
  const labelList& meshEdges = patch.meshEdges();
  const labelList& pFaces = patch.pointFaces()[pointI];
  // Vertices of dualFace
  DynamicList<label> dualFace{pFaces.size()};
  // Indices in dualFace of vertices added because of feature edge
  DynamicList<label> featEdgeIndices{dualFace.size()};
  label edgeI = startEdgeI;
  label faceI = patch.edgeFaces()[edgeI][0];
  // Check order of vertices of edgeI in face to see if we need to reverse.
  bool reverseFace;
  label e0, e1;
  getPointEdges(patch, faceI, pointI, e0, e1);
  if (e0 == edgeI) {
    reverseFace = true;
  } else {
    reverseFace = false;
  }
  while (true) {
    // Insert dual vertex for face
    dualFace.append(faceI + patchToDualOffset);
    // Cross face to other edge on pointI
    label e0, e1;
    getPointEdges(patch, faceI, pointI, e0, e1);
    if (e0 == edgeI) {
      edgeI = e1;
    } else {
      edgeI = e0;
    }
    if (edgeToDualPoint[meshEdges[edgeI]] >= 0) {
      // Feature edge. Insert dual vertex for edge.
      dualFace.append(edgeToDualPoint[meshEdges[edgeI]]);
      featEdgeIndices.append(dualFace.size()-1);
    }
    if (edgeI == startEdgeI) {
      break;
    }
    // Cross edge to other face.
    const labelList& eFaces = patch.edgeFaces()[edgeI];
    if (eFaces[0] == faceI) {
      faceI = eFaces[1];
    } else {
      faceI = eFaces[0];
    }
  }
  dualFace2.transfer(dualFace);
  featEdgeIndices2.transfer(featEdgeIndices);
  if (reverseFace) {
    reverse(dualFace2);
    // Correct featEdgeIndices for change in dualFace2
    FOR_ALL(featEdgeIndices2, i) {
      featEdgeIndices2[i] = dualFace2.size() -1 - featEdgeIndices2[i];
    }
    // Reverse indices (might not be necessary but do anyway)
    reverse(featEdgeIndices2);
  }
}


void mousse::polyDualMesh::splitFace
(
  const polyPatch& patch,
  const labelList& pointToDualPoint,
  const label pointI,
  const labelList& dualFace,
  const labelList& featEdgeIndices,
  DynamicList<face>& dualFaces,
  DynamicList<label>& dualOwner,
  DynamicList<label>& dualNeighbour,
  DynamicList<label>& dualRegion
)
{
  // Split face because of feature edges/points
  label meshPointI = patch.meshPoints()[pointI];
  if (pointToDualPoint[meshPointI] >= 0) {
    // Feature point. Do face-centre decomposition.
    if (featEdgeIndices.size() < 2) {
      // Feature point but no feature edges. Not handled at the moment
      dualFaces.append(face(dualFace));
      dualOwner.append(meshPointI);
      dualNeighbour.append(-1);
      dualRegion.append(patch.index());
    } else {
      // Do 'face-centre' decomposition. Start from first feature
      // edge create face up until next feature edge.
      FOR_ALL(featEdgeIndices, i) {
        label startFp = featEdgeIndices[i];
        label endFp = featEdgeIndices[(i+1) % featEdgeIndices.size()];
        // Collect face from startFp to endFp
        label sz = 0;
        if (endFp > startFp) {
          sz = endFp - startFp + 2;
        } else {
          sz = endFp + dualFace.size() - startFp + 2;
        }
        face subFace{sz};
        // feature point becomes face centre.
        subFace[0] = pointToDualPoint[patch.meshPoints()[pointI]];
        // Copy from startFp up to endFp.
        for (label subFp = 1; subFp < subFace.size(); subFp++) {
          subFace[subFp] = dualFace[startFp];
          startFp = (startFp+1) % dualFace.size();
        }
        dualFaces.append(face(subFace));
        dualOwner.append(meshPointI);
        dualNeighbour.append(-1);
        dualRegion.append(patch.index());
      }
    }
  } else {
    // No feature point. Check if feature edges.
    if (featEdgeIndices.size() < 2) {
      // Not enough feature edges. No split.
      dualFaces.append(face(dualFace));
      dualOwner.append(meshPointI);
      dualNeighbour.append(-1);
      dualRegion.append(patch.index());
    } else {
      // Multiple feature edges but no feature point.
      // Split face along feature edges. Gives weird result if
      // number of feature edges > 2.
      // Storage for new face
      DynamicList<label> subFace{dualFace.size()};
      FOR_ALL(featEdgeIndices, featI) {
        label startFp = featEdgeIndices[featI];
        label endFp = featEdgeIndices[featEdgeIndices.fcIndex(featI)];
        label fp = startFp;
        while (true) {
          label vertI = dualFace[fp];
          subFace.append(vertI);
          if (fp == endFp) {
            break;
          }
          fp = dualFace.fcIndex(fp);
        }
        if (subFace.size() > 2) {
          // Enough vertices to create a face from.
          subFace.shrink();
          dualFaces.append(face(subFace));
          dualOwner.append(meshPointI);
          dualNeighbour.append(-1);
          dualRegion.append(patch.index());
          subFace.clear();
        }
      }
      // Check final face.
      if (subFace.size() > 2) {
        // Enough vertices to create a face from.
        subFace.shrink();
        dualFaces.append(face(subFace));
        dualOwner.append(meshPointI);
        dualNeighbour.append(-1);
        dualRegion.append(patch.index());
        subFace.clear();
      }
    }
  }
}


// Create boundary face for every point in patch
void mousse::polyDualMesh::dualPatch
(
  const polyPatch& patch,
  const label patchToDualOffset,
  const labelList& edgeToDualPoint,
  const labelList& pointToDualPoint,
  const pointField& /*dualPoints*/,
  DynamicList<face>& dualFaces,
  DynamicList<label>& dualOwner,
  DynamicList<label>& dualNeighbour,
  DynamicList<label>& dualRegion
)
{
  const labelList& meshEdges = patch.meshEdges();
  // Whether edge has been done.
  // 0 : not
  // 1 : done e.start()
  // 2 : done e.end()
  // 3 : done both
  labelList doneEdgeSide{meshEdges.size(), 0};
  boolList donePoint{patch.nPoints(), false};
  // Do points on edge of patch
  FOR_ALL(doneEdgeSide, patchEdgeI) {
    const labelList& eFaces = patch.edgeFaces()[patchEdgeI];
    if (eFaces.size() == 1) {
      const edge& e = patch.edges()[patchEdgeI];
      FOR_ALL(e, eI) {
        label bitMask = 1 << eI;
        if ((doneEdgeSide[patchEdgeI] & bitMask) == 0) {
          // Construct face by walking around e[eI] starting from
          // patchEdgeI
          label pointI = e[eI];
          label edgeI = patchEdgeI;
          labelList dualFace
          {
            collectPatchSideFace
            (
              patch,
              patchToDualOffset,
              edgeToDualPoint,
              pointToDualPoint,
              pointI,
              edgeI
            )
          };
          // Now edgeI is end edge. Mark as visited
          if (patch.edges()[edgeI][0] == pointI) {
            doneEdgeSide[edgeI] |= 1;
          } else {
            doneEdgeSide[edgeI] |= 2;
          }
          dualFaces.append(face(dualFace));
          dualOwner.append(patch.meshPoints()[pointI]);
          dualNeighbour.append(-1);
          dualRegion.append(patch.index());
          doneEdgeSide[patchEdgeI] |= bitMask;
          donePoint[pointI] = true;
        }
      }
    }
  }
  // Do patch-internal points
  FOR_ALL(donePoint, pointI) {
    if (!donePoint[pointI]) {
      labelList dualFace, featEdgeIndices;
      collectPatchInternalFace
      (
        patch,
        patchToDualOffset,
        edgeToDualPoint,
        pointI,
        patch.pointEdges()[pointI][0],  // Arbitrary starting edge
        dualFace,
        featEdgeIndices
      );
      //- Either keep in one piece or split face according to feature.
      splitFace
      (
        patch,
        pointToDualPoint,
        pointI,
        dualFace,
        featEdgeIndices,
        dualFaces,
        dualOwner,
        dualNeighbour,
        dualRegion
      );
      donePoint[pointI] = true;
    }
  }
}


void mousse::polyDualMesh::calcDual
(
  const polyMesh& mesh,
  const labelList& featureEdges,
  const labelList& featurePoints
)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  const label nIntFaces = mesh.nInternalFaces();
  // Get patch for all of outside
  primitivePatch allBoundary
  {
    SubList<face>
    {
      mesh.faces(),
      mesh.nFaces() - nIntFaces,
      nIntFaces
    },
    mesh.points()
  };
  // Correspondence from patch edge to mesh edge.
  labelList meshEdges
  {
    allBoundary.meshEdges
    (
      mesh.edges(),
      mesh.pointEdges()
    )
  };

  {
    pointSet nonManifoldPoints
    {
      mesh,
      "nonManifoldPoints",
      meshEdges.size() / 100
    };
    allBoundary.checkPointManifold(true, &nonManifoldPoints);
    if (nonManifoldPoints.size()) {
      nonManifoldPoints.write();
      FATAL_ERROR_IN
      (
        "polyDualMesh::calcDual(const polyMesh&, const labelList&"
        ", const labelList&)"
      )   << "There are " << nonManifoldPoints.size() << " points where"
        << " the outside of the mesh is non-manifold." << nl
        << "Such a mesh cannot be converted to a dual." << nl
        << "Writing points at non-manifold sites to pointSet "
        << nonManifoldPoints.name()
        << exit(FatalError);
    }
  }
  // Assign points
  // mesh label   dualMesh vertex
  // ----------   ---------------
  // cellI        cellI
  // faceI        nCells+faceI-nIntFaces
  // featEdgeI    nCells+nFaces-nIntFaces+featEdgeI
  // featPointI   nCells+nFaces-nIntFaces+nFeatEdges+featPointI
  pointField dualPoints
  {
    mesh.nCells()                           // cell centres
    + mesh.nFaces() - nIntFaces               // boundary face centres
    + featureEdges.size()                     // additional boundary edges
    + featurePoints.size()                    // additional boundary points
  };
  label dualPointI = 0;
  // Cell centres.
  const pointField& cellCentres = mesh.cellCentres();
  cellPoint_.setSize(cellCentres.size());
  FOR_ALL(cellCentres, cellI) {
    cellPoint_[cellI] = dualPointI;
    dualPoints[dualPointI++] = cellCentres[cellI];
  }
  // Boundary faces centres
  const pointField& faceCentres = mesh.faceCentres();
  boundaryFacePoint_.setSize(mesh.nFaces() - nIntFaces);
  for (label faceI = nIntFaces; faceI < mesh.nFaces(); faceI++) {
    boundaryFacePoint_[faceI - nIntFaces] = dualPointI;
    dualPoints[dualPointI++] = faceCentres[faceI];
  }
  // Edge status:
  //  >0 : dual point label (edge is feature edge)
  //  -1 : is boundary edge.
  //  -2 : is internal edge.
  labelList edgeToDualPoint{mesh.nEdges(), -2};
  FOR_ALL(meshEdges, patchEdgeI) {
    label edgeI = meshEdges[patchEdgeI];
    edgeToDualPoint[edgeI] = -1;
  }
  FOR_ALL(featureEdges, i) {
    label edgeI = featureEdges[i];
    const edge& e = mesh.edges()[edgeI];
    edgeToDualPoint[edgeI] = dualPointI;
    dualPoints[dualPointI++] = e.centre(mesh.points());
  }
  // Point status:
  //  >0 : dual point label (point is feature point)
  //  -1 : is point on edge of patch
  //  -2 : is point on patch (but not on edge)
  //  -3 : is internal point.
  labelList pointToDualPoint{mesh.nPoints(), -3};
  FOR_ALL(patches, patchI) {
    const labelList& meshPoints = patches[patchI].meshPoints();
    FOR_ALL(meshPoints, i) {
      pointToDualPoint[meshPoints[i]] = -2;
    }
    const labelListList& loops = patches[patchI].edgeLoops();
    FOR_ALL(loops, i) {
      const labelList& loop = loops[i];
      FOR_ALL(loop, j) {
        pointToDualPoint[meshPoints[loop[j]]] = -1;
      }
    }
  }
  FOR_ALL(featurePoints, i) {
    label pointI = featurePoints[i];
    pointToDualPoint[pointI] = dualPointI;
    dualPoints[dualPointI++] = mesh.points()[pointI];
  }
  // Storage for new faces.
  // Dynamic sized since we don't know size.
  DynamicList<face> dynDualFaces{mesh.nEdges()};
  DynamicList<label> dynDualOwner{mesh.nEdges()};
  DynamicList<label> dynDualNeighbour{mesh.nEdges()};
  DynamicList<label> dynDualRegion{mesh.nEdges()};
  // Generate faces from edges on the boundary
  FOR_ALL(meshEdges, patchEdgeI) {
    label edgeI = meshEdges[patchEdgeI];
    const edge& e = mesh.edges()[edgeI];
    label owner = -1;
    label neighbour = -1;
    if (e[0] < e[1]) {
      owner = e[0];
      neighbour = e[1];
    } else {
      owner = e[1];
      neighbour = e[0];
    }
    // Find the boundary faces using the edge.
    const labelList& patchFaces = allBoundary.edgeFaces()[patchEdgeI];
    if (patchFaces.size() != 2) {
      FATAL_ERROR_IN("polyDualMesh::calcDual")
        << "Cannot handle edges with " << patchFaces.size()
        << " connected boundary faces."
        << abort(FatalError);
    }
    label face0 = patchFaces[0] + nIntFaces;
    const face& f0 = mesh.faces()[face0];
    label face1 = patchFaces[1] + nIntFaces;
    // We want to start walking from patchFaces[0] or patchFaces[1],
    // depending on which one uses owner,neighbour in the right order.
    label startFaceI = -1;
    label endFaceI = -1;
    label index = findIndex(f0, neighbour);
    if (f0.nextLabel(index) == owner) {
      startFaceI = face0;
      endFaceI = face1;
    } else {
      startFaceI = face1;
      endFaceI = face0;
    }
    // Now walk from startFaceI to cell to face to cell etc. until endFaceI
    DynamicList<label> dualFace;
    if (edgeToDualPoint[edgeI] >= 0) {
      // Number of cells + 2 boundary faces + feature edge point
      dualFace.setCapacity(mesh.edgeCells()[edgeI].size()+2+1);
      // Store dualVertex for feature edge
      dualFace.append(edgeToDualPoint[edgeI]);
    } else {
      dualFace.setCapacity(mesh.edgeCells()[edgeI].size()+2);
    }
    // Store dual vertex for starting face.
    dualFace.append(mesh.nCells() + startFaceI - nIntFaces);
    label cellI = mesh.faceOwner()[startFaceI];
    label faceI = startFaceI;
    while (true) {
      // Store dual vertex from cellI.
      dualFace.append(cellI);
      // Cross cell to other face on edge.
      label f0, f1;
      meshTools::getEdgeFaces(mesh, cellI, edgeI, f0, f1);
      if (f0 == faceI) {
        faceI = f1;
      } else {
        faceI = f0;
      }
      // Cross face to other cell.
      if (faceI == endFaceI) {
        break;
      }
      if (mesh.faceOwner()[faceI] == cellI) {
        cellI = mesh.faceNeighbour()[faceI];
      } else {
        cellI = mesh.faceOwner()[faceI];
      }
    }
    // Store dual vertex for endFace.
    dualFace.append(mesh.nCells() + endFaceI - nIntFaces);
    dynDualFaces.append(face(dualFace.shrink()));
    dynDualOwner.append(owner);
    dynDualNeighbour.append(neighbour);
    dynDualRegion.append(-1);

    {
      // Check orientation.
      const face& f = dynDualFaces.last();
      vector n = f.normal(dualPoints);
      if (((mesh.points()[owner] - dualPoints[f[0]]) & n) > 0) {
        WARNING_IN("calcDual") << "Incorrect orientation"
          << " on boundary edge:" << edgeI
          << mesh.points()[mesh.edges()[edgeI][0]]
          << mesh.points()[mesh.edges()[edgeI][1]]
          << endl;
      }
    }

  }
  // Generate faces from internal edges
  FOR_ALL(edgeToDualPoint, edgeI) {
    if (edgeToDualPoint[edgeI] == -2) {
      // Internal edge.
      // Find dual owner, neighbour
      const edge& e = mesh.edges()[edgeI];
      label owner = -1;
      label neighbour = -1;
      if (e[0] < e[1]) {
        owner = e[0];
        neighbour = e[1];
      } else {
        owner = e[1];
        neighbour = e[0];
      }
      // Get a starting cell
      const labelList& eCells = mesh.edgeCells()[edgeI];
      label cellI = eCells[0];
      // Get the two faces on the cell and edge.
      label face0, face1;
      meshTools::getEdgeFaces(mesh, cellI, edgeI, face0, face1);
      // Find the starting face by looking at the order in which
      // the face uses the owner, neighbour
      const face& f0 = mesh.faces()[face0];
      label index = findIndex(f0, neighbour);
      bool f0OrderOk = (f0.nextLabel(index) == owner);
      label startFaceI = -1;
      if (f0OrderOk == (mesh.faceOwner()[face0] == cellI)) {
        startFaceI = face0;
      } else {
        startFaceI = face1;
      }
      // Walk face-cell-face until starting face reached.
      DynamicList<label> dualFace{mesh.edgeCells()[edgeI].size()};
      label faceI = startFaceI;
      while (true) {
        // Store dual vertex from cellI.
        dualFace.append(cellI);
        // Cross cell to other face on edge.
        label f0, f1;
        meshTools::getEdgeFaces(mesh, cellI, edgeI, f0, f1);
        if (f0 == faceI) {
          faceI = f1;
        } else {
          faceI = f0;
        }
        // Cross face to other cell.
        if (faceI == startFaceI) {
          break;
        }
        if (mesh.faceOwner()[faceI] == cellI) {
          cellI = mesh.faceNeighbour()[faceI];
        } else {
          cellI = mesh.faceOwner()[faceI];
        }
      }

      dynDualFaces.append(face(dualFace.shrink()));
      dynDualOwner.append(owner);
      dynDualNeighbour.append(neighbour);
      dynDualRegion.append(-1);

      {
        // Check orientation.
        const face& f = dynDualFaces.last();
        vector n = f.normal(dualPoints);
        if (((mesh.points()[owner] - dualPoints[f[0]]) & n) > 0) {
          WARNING_IN("calcDual") << "Incorrect orientation"
            << " on internal edge:" << edgeI
            << mesh.points()[mesh.edges()[edgeI][0]]
            << mesh.points()[mesh.edges()[edgeI][1]]
            << endl;
        }
      }
    }
  }
  // Dump faces.
  if (debug) {
    dynDualFaces.shrink();
    dynDualOwner.shrink();
    dynDualNeighbour.shrink();
    dynDualRegion.shrink();
    OFstream str{"dualInternalFaces.obj"};
    Pout << "polyDualMesh::calcDual : dumping internal faces to "
      << str.name() << endl;
    FOR_ALL(dualPoints, dualPointI) {
      meshTools::writeOBJ(str, dualPoints[dualPointI]);
    }
    FOR_ALL(dynDualFaces, dualFaceI) {
      const face& f = dynDualFaces[dualFaceI];
      str << 'f';
      FOR_ALL(f, fp) {
        str << ' ' << f[fp]+1;
      }
      str<< nl;
    }
  }
  const label nInternalFaces = dynDualFaces.size();
  // Outside faces
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    dualPatch
    (
      pp,
      mesh.nCells() + pp.start() - nIntFaces,
      edgeToDualPoint,
      pointToDualPoint,
      dualPoints,
      dynDualFaces,
      dynDualOwner,
      dynDualNeighbour,
      dynDualRegion
    );
  }
  // Transfer face info to straight lists
  faceList dualFaces{dynDualFaces.shrink(), true};
  dynDualFaces.clear();
  labelList dualOwner{dynDualOwner.shrink(), true};
  dynDualOwner.clear();
  labelList dualNeighbour{dynDualNeighbour.shrink(), true};
  dynDualNeighbour.clear();
  labelList dualRegion{dynDualRegion.shrink(), true};
  dynDualRegion.clear();
  // Dump faces.
  if (debug) {
    OFstream str{"dualFaces.obj"};
    Pout << "polyDualMesh::calcDual : dumping all faces to " << str.name()
      << endl;
    FOR_ALL(dualPoints, dualPointI) {
      meshTools::writeOBJ(str, dualPoints[dualPointI]);
    }
    FOR_ALL(dualFaces, dualFaceI) {
      const face& f = dualFaces[dualFaceI];
      str << 'f';
      FOR_ALL(f, fp) {
        str << ' ' << f[fp]+1;
      }
      str << nl;
    }
  }
  // Create cells.
  cellList dualCells{mesh.nPoints()};
  FOR_ALL(dualCells, cellI) {
    dualCells[cellI].setSize(0);
  }
  FOR_ALL(dualOwner, faceI) {
    label cellI = dualOwner[faceI];
    labelList& cFaces = dualCells[cellI];
    label sz = cFaces.size();
    cFaces.setSize(sz+1);
    cFaces[sz] = faceI;
  }
  FOR_ALL(dualNeighbour, faceI) {
    label cellI = dualNeighbour[faceI];
    if (cellI != -1) {
      labelList& cFaces = dualCells[cellI];
      label sz = cFaces.size();
      cFaces.setSize(sz+1);
      cFaces[sz] = faceI;
    }
  }
  // Do upper-triangular face ordering. Determines face reordering map and
  // number of internal faces.
  label dummy;
  labelList oldToNew
  {
    getFaceOrder
    (
      dualOwner,
      dualNeighbour,
      dualCells,
      dummy
    )
  };
  // Reorder faces.
  inplaceReorder(oldToNew, dualFaces);
  inplaceReorder(oldToNew, dualOwner);
  inplaceReorder(oldToNew, dualNeighbour);
  inplaceReorder(oldToNew, dualRegion);
  FOR_ALL(dualCells, cellI) {
    inplaceRenumber(oldToNew, dualCells[cellI]);
  }
  // Create patches
  labelList patchSizes{patches.size(), 0};
  FOR_ALL(dualRegion, faceI) {
    if (dualRegion[faceI] >= 0) {
      patchSizes[dualRegion[faceI]]++;
    }
  }
  labelList patchStarts{patches.size(), 0};
  label faceI = nInternalFaces;
  FOR_ALL(patches, patchI) {
    patchStarts[patchI] = faceI;
    faceI += patchSizes[patchI];
  }
  Pout << "nFaces:" << dualFaces.size()
    << " patchSizes:" << patchSizes
    << " patchStarts:" << patchStarts
    << endl;
  // Add patches. First add zero sized (since mesh still 0 size)
  List<polyPatch*> dualPatches{patches.size()};
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    dualPatches[patchI] = pp.clone
    (
      boundaryMesh(),
      patchI,
      0, //patchSizes[patchI],
      0  //patchStarts[patchI]
    ).ptr();
  }
  addPatches(dualPatches);
  // Assign to mesh.
  resetPrimitives
  (
    xferMove(dualPoints),
    xferMove(dualFaces),
    xferMove(dualOwner),
    xferMove(dualNeighbour),
    patchSizes,
    patchStarts
  );
}


// Constructors 

// Construct from components
mousse::polyDualMesh::polyDualMesh(const IOobject& io)
:
  polyMesh{io},
  cellPoint_
  {
    {
      "cellPoint",
      time().findInstance(meshDir(), "cellPoint"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  boundaryFacePoint_
  {
    {
      "boundaryFacePoint",
      time().findInstance(meshDir(), "boundaryFacePoint"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  }
{}


// Construct from polyMesh
mousse::polyDualMesh::polyDualMesh
(
  const polyMesh& mesh,
  const labelList& featureEdges,
  const labelList& featurePoints
)
:
  polyMesh
  {
    mesh,
    xferCopy(pointField()),// to prevent any warnings "points not allocated"
    xferCopy(faceList()),  // to prevent any warnings "faces  not allocated"
    xferCopy(cellList())
  },
  cellPoint_
  {
    {
      "cellPoint",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nCells(), -1}
  },
  boundaryFacePoint_
  {
    {
      "boundaryFacePoint",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nFaces() - mesh.nInternalFaces()}
  }
{
  calcDual(mesh, featureEdges, featurePoints);
}


// Construct from polyMesh and feature angle
mousse::polyDualMesh::polyDualMesh
(
  const polyMesh& mesh,
  const scalar featureCos
)
:
  polyMesh
  {
    mesh,
    xferCopy(pointField()),// to prevent any warnings "points not allocated"
    xferCopy(faceList()),  // to prevent any warnings "faces  not allocated"
    xferCopy(cellList())
  },
  cellPoint_
  {
    {
      "cellPoint",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nCells(), -1}
  },
  boundaryFacePoint_
  {
    {
      "boundaryFacePoint",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nFaces() - mesh.nInternalFaces(), -1}
  }
{
  labelList featureEdges, featurePoints;
  calcFeatures(mesh, featureCos, featureEdges, featurePoints);
  calcDual(mesh, featureEdges, featurePoints);
}


void mousse::polyDualMesh::calcFeatures
(
  const polyMesh& mesh,
  const scalar featureCos,
  labelList& featureEdges,
  labelList& featurePoints
)
{
  // Create big primitivePatch for all outside.
  primitivePatch allBoundary
  {
    SubList<face>
    {
      mesh.faces(),
      mesh.nFaces() - mesh.nInternalFaces(),
      mesh.nInternalFaces()
    },
    mesh.points()
  };
  // For ease of use store patch number per face in allBoundary.
  labelList allRegion{allBoundary.size()};
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    FOR_ALL(pp, i) {
      allRegion[i + pp.start() - mesh.nInternalFaces()] = patchI;
    }
  }
  // Calculate patch/feature edges
  const labelListList& edgeFaces = allBoundary.edgeFaces();
  const vectorField& faceNormals = allBoundary.faceNormals();
  const labelList& meshPoints = allBoundary.meshPoints();
  boolList isFeatureEdge{edgeFaces.size(), false};
  FOR_ALL(edgeFaces, edgeI) {
    const labelList& eFaces = edgeFaces[edgeI];
    if (eFaces.size() != 2) {
      // Non-manifold. Problem?
      const edge& e = allBoundary.edges()[edgeI];
      WARNING_IN("polyDualMesh::calcFeatures") << "Edge "
        << meshPoints[e[0]] << ' ' << meshPoints[e[1]]
        << "  coords:" << mesh.points()[meshPoints[e[0]]]
        << mesh.points()[meshPoints[e[1]]]
        << " has more than 2 faces connected to it:"
        << eFaces.size() << endl;
      isFeatureEdge[edgeI] = true;
    } else if (allRegion[eFaces[0]] != allRegion[eFaces[1]]) {
      isFeatureEdge[edgeI] = true;
    } else if ((faceNormals[eFaces[0]] & faceNormals[eFaces[1]]) < featureCos) {
      isFeatureEdge[edgeI] = true;
    }
  }
  // Calculate feature points
  const labelListList& pointEdges = allBoundary.pointEdges();
  DynamicList<label> allFeaturePoints{pointEdges.size()};
  FOR_ALL(pointEdges, pointI) {
    const labelList& pEdges = pointEdges[pointI];
    label nFeatEdges = 0;
    FOR_ALL(pEdges, i) {
      if (isFeatureEdge[pEdges[i]]) {
        nFeatEdges++;
      }
    }
    if (nFeatEdges > 2) {
      // Store in mesh vertex label
      allFeaturePoints.append(allBoundary.meshPoints()[pointI]);
    }
  }
  featurePoints.transfer(allFeaturePoints);
  if (debug) {
    OFstream str{"featurePoints.obj"};
    Pout << "polyDualMesh::calcFeatures : dumping feature points to "
      << str.name() << endl;
    FOR_ALL(featurePoints, i) {
      label pointI = featurePoints[i];
      meshTools::writeOBJ(str, mesh.points()[pointI]);
    }
  }
  // Get all feature edges.
  labelList meshEdges
  {
    allBoundary.meshEdges
    (
      mesh.edges(),
      mesh.cellEdges(),
      SubList<label>
      {
        mesh.faceOwner(),
        allBoundary.size(),
        mesh.nInternalFaces()
      }
    )
  };
  DynamicList<label> allFeatureEdges{isFeatureEdge.size()};
  FOR_ALL(isFeatureEdge, edgeI) {
    if (isFeatureEdge[edgeI]) {
      // Store in mesh edge label.
      allFeatureEdges.append(meshEdges[edgeI]);
    }
  }
  featureEdges.transfer(allFeatureEdges);
}


// Destructor 
mousse::polyDualMesh::~polyDualMesh()
{}

