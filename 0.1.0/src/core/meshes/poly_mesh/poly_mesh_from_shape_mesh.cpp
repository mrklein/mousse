// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "time.hpp"
#include "primitive_mesh.hpp"
#include "dynamic_list.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"
#include "global_mesh_data.hpp"
#include "pstream_reduce_ops.hpp"


// Private Member Functions 
mousse::labelListList mousse::polyMesh::cellShapePointCells
(
  const cellShapeList& c
) const
{
  List<DynamicList<label, primitiveMesh::cellsPerPoint_>> pc(points().size());
  // For each cell
  FOR_ALL(c, i) {
    // For each vertex
    const labelList& labels = c[i];
    FOR_ALL(labels, j) {
      // Set working point label
      label curPoint = labels[j];
      DynamicList<label, primitiveMesh::cellsPerPoint_>& curPointCells =
        pc[curPoint];
      // Enter the cell label in the point's cell list
      curPointCells.append(i);
    }
  }
  labelListList pointCellAddr{pc.size()};
  FOR_ALL(pc, pointI) {
    pointCellAddr[pointI].transfer(pc[pointI]);
  }
  return pointCellAddr;
}


mousse::labelList mousse::polyMesh::facePatchFaceCells
(
  const faceList& patchFaces,
  const labelListList& pointCells,
  const faceListList& cellsFaceShapes,
  const label patchID
) const
{
  bool found;
  labelList FaceCells{patchFaces.size()};
  FOR_ALL(patchFaces, fI) {
    found = false;
    const face& curFace = patchFaces[fI];
    const labelList& facePoints = patchFaces[fI];
    FOR_ALL(facePoints, pointI) {
      const labelList& facePointCells = pointCells[facePoints[pointI]];
      FOR_ALL(facePointCells, cellI) {
        faceList cellFaces = cellsFaceShapes[facePointCells[cellI]];
        FOR_ALL(cellFaces, cellFace) {
          if (face::sameVertices(cellFaces[cellFace], curFace)) {
            // Found the cell corresponding to this face
            FaceCells[fI] = facePointCells[cellI];
            found = true;
          }
          if (found)
            break;
        }
        if (found)
          break;
      }
      if (found)
        break;
    }
    if (!found) {
      FATAL_ERROR_IN
      (
        "polyMesh::facePatchFaceCells(const faceList& patchFaces,"
        "const labelListList& pointCells,"
        "const faceListList& cellsFaceShapes,"
        "const label patchID)"
      )
      << "face " << fI << " in patch " << patchID
      << " does not have neighbour cell"
      << " face: " << patchFaces[fI]
      << abort(FatalError);
    }
  }
  return FaceCells;
}


//- Set faces_, calculate cells and patchStarts.
void mousse::polyMesh::setTopology
(
  const cellShapeList& cellsAsShapes,
  const faceListList& boundaryFaces,
  const wordList& boundaryPatchNames,
  labelList& patchSizes,
  labelList& patchStarts,
  label& defaultPatchStart,
  label& nFaces,
  cellList& cells
)
{
  // Calculate the faces of all cells
  // Initialise maximum possible numer of mesh faces to 0
  label maxFaces = 0;
  // Set up a list of face shapes for each cell
  faceListList cellsFaceShapes{cellsAsShapes.size()};
  cells.setSize(cellsAsShapes.size());
  FOR_ALL(cellsFaceShapes, cellI) {
    cellsFaceShapes[cellI] = cellsAsShapes[cellI].faces();
    cells[cellI].setSize(cellsFaceShapes[cellI].size());
    // Initialise cells to -1 to flag undefined faces
    static_cast<labelList&>(cells[cellI]) = -1;
    // Count maximum possible numer of mesh faces
    maxFaces += cellsFaceShapes[cellI].size();
  }
  // Set size of faces array to maximum possible number of mesh faces
  faces_.setSize(maxFaces);
  // Initialise number of faces to 0
  nFaces = 0;
  // Set reference to point-cell addressing
  labelListList PointCells = cellShapePointCells(cellsAsShapes);
  bool found = false;
  FOR_ALL(cells, cellI) {
    // Note:
    // Insertion cannot be done in one go as the faces need to be
    // added into the list in the increasing order of neighbour
    // cells.  Therefore, all neighbours will be detected first
    // and then added in the correct order.
    const faceList& curFaces = cellsFaceShapes[cellI];
    // Record the neighbour cell
    labelList neiCells{curFaces.size(), -1};
    // Record the face of neighbour cell
    labelList faceOfNeiCell{curFaces.size(), -1};
    label nNeighbours = 0;
    // For all faces ...
    FOR_ALL(curFaces, faceI) {
      // Skip faces that have already been matched
      if (cells[cellI][faceI] >= 0)
        continue;
      found = false;
      const face& curFace = curFaces[faceI];
      // Get the list of labels
      const labelList& curPoints = curFace;
      // For all points
      FOR_ALL(curPoints, pointI) {
        // dGget the list of cells sharing this point
        const labelList& curNeighbours = PointCells[curPoints[pointI]];
        // For all neighbours
        FOR_ALL(curNeighbours, neiI) {
          label curNei = curNeighbours[neiI];
          // Reject neighbours with the lower label
          if (curNei > cellI) {
            // Get the list of search faces
            const faceList& searchFaces = cellsFaceShapes[curNei];
            FOR_ALL(searchFaces, neiFaceI) {
              if (searchFaces[neiFaceI] == curFace) {
                // Match!!
                found = true;
                // Record the neighbour cell and face
                neiCells[faceI] = curNei;
                faceOfNeiCell[faceI] = neiFaceI;
                nNeighbours++;
                break;
              }
            }
            if (found)
              break;
          }
          if (found)
            break;
        }
        if (found)
          break;
      } // End of current points
    }  // End of current faces
    // Add the faces in the increasing order of neighbours
    for (label neiSearch = 0; neiSearch < nNeighbours; neiSearch++) {
      // Find the lowest neighbour which is still valid
      label nextNei = -1;
      label minNei = cells.size();
      FOR_ALL(neiCells, ncI) {
        if (neiCells[ncI] > -1 && neiCells[ncI] < minNei) {
          nextNei = ncI;
          minNei = neiCells[ncI];
        }
      }
      if (nextNei > -1) {
        // Add the face to the list of faces
        faces_[nFaces] = curFaces[nextNei];
        // Set cell-face and cell-neighbour-face to current face label
        cells[cellI][nextNei] = nFaces;
        cells[neiCells[nextNei]][faceOfNeiCell[nextNei]] = nFaces;
        // Stop the neighbour from being used again
        neiCells[nextNei] = -1;
        // Increment number of faces counter
        nFaces++;
      } else {
        FATAL_ERROR_IN
        (
          "polyMesh::setTopology\n"
          "(\n"
          "    const cellShapeList& cellsAsShapes,\n"
          "    const faceListList& boundaryFaces,\n"
          "    const wordList& boundaryPatchNames,\n"
          "    labelList& patchSizes,\n"
          "    labelList& patchStarts,\n"
          "    label& defaultPatchStart,\n"
          "    label& nFaces,\n"
          "    cellList& cells\n"
          ")"
        )
        << "Error in internal face insertion"
        << abort(FatalError);
      }
    }
  }
  // Do boundary faces
  patchSizes.setSize(boundaryFaces.size(), -1);
  patchStarts.setSize(boundaryFaces.size(), -1);
  FOR_ALL(boundaryFaces, patchI) {
    const faceList& patchFaces = boundaryFaces[patchI];
    labelList curPatchFaceCells =
      facePatchFaceCells
      (
        patchFaces,
        PointCells,
        cellsFaceShapes,
        patchI
      );
    // Grab the start label
    label curPatchStart = nFaces;
    FOR_ALL(patchFaces, faceI) {
      const face& curFace = patchFaces[faceI];
      const label cellInside = curPatchFaceCells[faceI];
      // Get faces of the cell inside
      const faceList& facesOfCellInside = cellsFaceShapes[cellInside];
      bool found = false;
      FOR_ALL(facesOfCellInside, cellFaceI) {
        if (face::sameVertices(facesOfCellInside[cellFaceI], curFace)) {
          if (cells[cellInside][cellFaceI] >= 0) {
            FATAL_ERROR_IN
            (
              "polyMesh::setTopology\n"
              "(\n"
              "    const cellShapeList& cellsAsShapes,\n"
              "    const faceListList& boundaryFaces,\n"
              "    const wordList& boundaryPatchNames,\n"
              "    labelList& patchSizes,\n"
              "    labelList& patchStarts,\n"
              "    label& defaultPatchStart,\n"
              "    label& nFaces,\n"
              "    cellList& cells\n"
              ")"
            )
            << "Trying to specify a boundary face " << curFace
            << " on the face on cell " << cellInside
            << " which is either an internal face or already "
            << "belongs to some other patch.  This is face "
            << faceI << " of patch "
            << patchI << " named "
            << boundaryPatchNames[patchI] << "."
            << abort(FatalError);
          }
          found = true;
          // Set the patch face to corresponding cell-face
          faces_[nFaces] = facesOfCellInside[cellFaceI];
          cells[cellInside][cellFaceI] = nFaces;
          break;
        }
      }
      if (!found) {
        FATAL_ERROR_IN("polyMesh::polyMesh(... construct from shapes...)")
          << "face " << faceI << " of patch " << patchI
          << " does not seem to belong to cell " << cellInside
          << " which, according to the addressing, "
          << "should be next to it."
          << abort(FatalError);
      }
      // Increment the counter of faces
      nFaces++;
    }
    patchSizes[patchI] = nFaces - curPatchStart;
    patchStarts[patchI] = curPatchStart;
  }
  // Grab "non-existing" faces and put them into a default patch
  defaultPatchStart = nFaces;
  FOR_ALL(cells, cellI) {
    labelList& curCellFaces = cells[cellI];
    FOR_ALL(curCellFaces, faceI) {
      if (curCellFaces[faceI] == -1) { // "non-existent" face
        curCellFaces[faceI] = nFaces;
        faces_[nFaces] = cellsFaceShapes[cellI][faceI];
        nFaces++;
      }
    }
  }
  // Reset the size of the face list
  faces_.setSize(nFaces);
  return ;
}


mousse::polyMesh::polyMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const cellShapeList& cellsAsShapes,
  const faceListList& boundaryFaces,
  const wordList& boundaryPatchNames,
  const wordList& boundaryPatchTypes,
  const word& defaultBoundaryPatchName,
  const word& defaultBoundaryPatchType,
  const wordList& boundaryPatchPhysicalTypes,
  const bool syncPar
)
:
  objectRegistry{io},
  primitiveMesh{},
  points_
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    points
  },
  faces_
  {
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  owner_
  {
    {
      "owner",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  neighbour_
  {
    {
      "neighbour",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  clearedPrimitives_{false},
  boundary_
  {
    {
      "boundary",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    *this,
    boundaryFaces.size() + 1    // Add room for a default patch
  },
  bounds_{points_, syncPar},
  comm_{UPstream::worldComm},
  geometricD_{Vector<label>::zero},
  solutionD_{Vector<label>::zero},
  tetBasePtIsPtr_{NULL},
  cellTreePtr_{NULL},
  pointZones_
  {
    {
      "pointZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  faceZones_
  {
    {
      "faceZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  cellZones_
  {
    {
      "cellZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  globalMeshDataPtr_{NULL},
  moving_{false},
  topoChanging_{false},
  curMotionTimeIndex_{time().timeIndex()},
  oldPointsPtr_{NULL}
{
  if (debug) {
    Info << "Constructing polyMesh from cell and boundary shapes." << endl;
  }
  // Remove all of the old mesh files if they exist
  removeFiles(instance());
  // Calculate faces and cells
  labelList patchSizes;
  labelList patchStarts;
  label defaultPatchStart;
  label nFaces;
  cellList cells;
  setTopology
  (
    cellsAsShapes,
    boundaryFaces,
    boundaryPatchNames,
    patchSizes,
    patchStarts,
    defaultPatchStart,
    nFaces,
    cells
  );
  // Warning: Patches can only be added once the face list is
  // completed, as they hold a subList of the face list
  FOR_ALL(boundaryFaces, patchI) {
    // Add the patch to the list
    boundary_.set
    (
      patchI,
      polyPatch::New
      (
        boundaryPatchTypes[patchI],
        boundaryPatchNames[patchI],
        patchSizes[patchI],
        patchStarts[patchI],
        patchI,
        boundary_
      )
    );
    if (boundaryPatchPhysicalTypes.size()
        && boundaryPatchPhysicalTypes[patchI].size()) {
      boundary_[patchI].physicalType() = boundaryPatchPhysicalTypes[patchI];
    }
  }
  label nAllPatches = boundaryFaces.size();
  label nDefaultFaces = nFaces - defaultPatchStart;
  if (syncPar) {
    reduce(nDefaultFaces, sumOp<label>());
  }
  if (nDefaultFaces > 0) {
    WARNING_IN("polyMesh::polyMesh(... construct from shapes...)")
      << "Found " << nDefaultFaces
      << " undefined faces in mesh; adding to default patch." << endl;
    // Check if there already exists a defaultFaces patch as last patch
    // and reuse it.
    label patchI = findIndex(boundaryPatchNames, defaultBoundaryPatchName);
    if (patchI != -1) {
      if (patchI != boundaryFaces.size()-1 || boundary_[patchI].size()) {
        FATAL_ERROR_IN("polyMesh::polyMesh(... construct from shapes...)")
          << "Default patch " << boundary_[patchI].name()
          << " already has faces in it or is not"
          << " last in list of patches." << exit(FatalError);
      }
      WARNING_IN("polyMesh::polyMesh(... construct from shapes...)")
        << "Reusing existing patch " << patchI
        << " for undefined faces." << endl;
      boundary_.set
      (
        patchI,
        polyPatch::New
        (
          boundaryPatchTypes[patchI],
          boundaryPatchNames[patchI],
          nFaces - defaultPatchStart,
          defaultPatchStart,
          patchI,
          boundary_
        )
      );
    } else {
      boundary_.set
      (
        nAllPatches,
        polyPatch::New
        (
          defaultBoundaryPatchType,
          defaultBoundaryPatchName,
          nFaces - defaultPatchStart,
          defaultPatchStart,
          boundary_.size() - 1,
          boundary_
        )
      );
      nAllPatches++;
    }
  }
  // Reset the size of the boundary
  boundary_.setSize(nAllPatches);
  // Set the primitive mesh
  initMesh(cells);
  if (syncPar) {
    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();
    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
  }
  if (debug) {
    if (checkMesh()) {
      Info << "Mesh OK" << endl;
    }
  }
}


mousse::polyMesh::polyMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const cellShapeList& cellsAsShapes,
  const faceListList& boundaryFaces,
  const wordList& boundaryPatchNames,
  const PtrList<dictionary>& boundaryDicts,
  const word& defaultBoundaryPatchName,
  const word& defaultBoundaryPatchType,
  const bool syncPar
)
:
  objectRegistry{io},
  primitiveMesh{},
  points_
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    points
  },
  faces_
  {
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  owner_
  {
    {
      "owner",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  neighbour_
  {
    {
      "neighbour",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  clearedPrimitives_{false},
  boundary_
  {
    {
      "boundary",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    *this,
    boundaryFaces.size() + 1    // Add room for a default patch
  },
  bounds_{points_, syncPar},
  comm_{UPstream::worldComm},
  geometricD_{Vector<label>::zero},
  solutionD_{Vector<label>::zero},
  tetBasePtIsPtr_{NULL},
  cellTreePtr_{NULL},
  pointZones_
  {
    {
      "pointZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  faceZones_
  {
    {
      "faceZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  cellZones_
  {
    {
      "cellZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  globalMeshDataPtr_{NULL},
  moving_{false},
  topoChanging_{false},
  curMotionTimeIndex_{time().timeIndex()},
  oldPointsPtr_{NULL}
{
  if (debug) {
    Info << "Constructing polyMesh from cell and boundary shapes." << endl;
  }
  // Remove all of the old mesh files if they exist
  removeFiles(instance());
  // Calculate faces and cells
  labelList patchSizes;
  labelList patchStarts;
  label defaultPatchStart;
  label nFaces;
  cellList cells;
  setTopology
  (
    cellsAsShapes,
    boundaryFaces,
    boundaryPatchNames,
    patchSizes,
    patchStarts,
    defaultPatchStart,
    nFaces,
    cells
  );
  // Warning: Patches can only be added once the face list is
  // completed, as they hold a subList of the face list
  FOR_ALL(boundaryDicts, patchI) {
    dictionary patchDict{boundaryDicts[patchI]};
    patchDict.set("nFaces", patchSizes[patchI]);
    patchDict.set("startFace", patchStarts[patchI]);
    // Add the patch to the list
    boundary_.set
    (
      patchI,
      polyPatch::New
      (
        boundaryPatchNames[patchI],
        patchDict,
        patchI,
        boundary_
      )
    );
  }
  label nAllPatches = boundaryFaces.size();
  label nDefaultFaces = nFaces - defaultPatchStart;
  if (syncPar) {
    reduce(nDefaultFaces, sumOp<label>());
  }
  if (nDefaultFaces > 0) {
    WARNING_IN("polyMesh::polyMesh(... construct from shapes...)")
      << "Found " << nDefaultFaces
      << " undefined faces in mesh; adding to default patch." << endl;
    // Check if there already exists a defaultFaces patch as last patch
    // and reuse it.
    label patchI = findIndex(boundaryPatchNames, defaultBoundaryPatchName);
    if (patchI != -1) {
      if (patchI != boundaryFaces.size()-1 || boundary_[patchI].size()) {
        FATAL_ERROR_IN("polyMesh::polyMesh(... construct from shapes...)")
          << "Default patch " << boundary_[patchI].name()
          << " already has faces in it or is not"
          << " last in list of patches." << exit(FatalError);
      }
      WARNING_IN("polyMesh::polyMesh(... construct from shapes...)")
        << "Reusing existing patch " << patchI
        << " for undefined faces." << endl;
      boundary_.set
      (
        patchI,
        polyPatch::New
        (
          boundary_[patchI].type(),
          boundary_[patchI].name(),
          nFaces - defaultPatchStart,
          defaultPatchStart,
          patchI,
          boundary_
        )
      );
    } else {
      boundary_.set
      (
        nAllPatches,
        polyPatch::New
        (
          defaultBoundaryPatchType,
          defaultBoundaryPatchName,
          nFaces - defaultPatchStart,
          defaultPatchStart,
          boundary_.size() - 1,
          boundary_
        )
      );
      nAllPatches++;
    }
  }
  // Reset the size of the boundary
  boundary_.setSize(nAllPatches);
  // Set the primitive mesh
  initMesh(cells);
  if (syncPar) {
    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();
    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
  }
  if (debug) {
    if (checkMesh()) {
      Info << "Mesh OK" << endl;
    }
  }
}
