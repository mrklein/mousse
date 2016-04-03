// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_sets.hpp"
#include "poly_mesh.hpp"
#include "tri_surface.hpp"
#include "tri_surface_search.hpp"
#include "point_set.hpp"
#include "cell_set.hpp"
#include "surface_to_cell.hpp"
#include "cell_to_point.hpp"
#include "cell_to_cell.hpp"
#include "point_to_cell.hpp"
#include "mesh_search.hpp"
#include "cell_classification.hpp"


// Member Functions 
void mousse::surfaceSets::getSurfaceSets
(
  const polyMesh& mesh,
  const fileName&,
  const triSurface&,
  const triSurfaceSearch& querySurf,
  const pointField& outsidePts,
  const label nCutLayers,
  labelHashSet& inside,
  labelHashSet& outside,
  labelHashSet& cut
)
{
  // Construct search engine on mesh
  meshSearch queryMesh{mesh};
  // Cut faces with surface and classify cells
  cellClassification cellType
  {
    mesh,
    queryMesh,
    querySurf,
    outsidePts
  };
  if (nCutLayers > 0) {
    // Trim cutCells so they are max nCutLayers away (as seen in point-cell
    // walk) from outside cells.
    cellType.trimCutCells
    (
      nCutLayers,
      cellClassification::OUTSIDE,
      cellClassification::INSIDE
    );
  }
  FOR_ALL(cellType, cellI) {
    label cType = cellType[cellI];
    if (cType == cellClassification::CUT) {
      cut.insert(cellI);
    } else if (cType == cellClassification::INSIDE) {
      inside.insert(cellI);
    } else if (cType == cellClassification::OUTSIDE) {
      outside.insert(cellI);
    }
  }
}


mousse::labelHashSet mousse::surfaceSets::getHangingCells
(
  const primitiveMesh& mesh,
  const labelHashSet& internalCells
)
{
  const cellList& cells = mesh.cells();
  const faceList& faces = mesh.faces();
  // Divide points into
  // -referenced by internal only
  // -referenced by outside only
  // -mixed
  List<pointStatus> pointSide{mesh.nPoints(), NOTSET};
  for (label cellI = 0; cellI < mesh.nCells(); cellI++) {
    if (internalCells.found(cellI)) {
      // Inside cell. Mark all vertices seen from this cell.
      const labelList& cFaces = cells[cellI];
      FOR_ALL(cFaces, cFaceI) {
        const face& f = faces[cFaces[cFaceI]];
        FOR_ALL(f, fp) {
          label pointI = f[fp];
          if (pointSide[pointI] == NOTSET) {
            pointSide[pointI] = INSIDE;
          } else if (pointSide[pointI] == OUTSIDE) {
            pointSide[pointI] = MIXED;
          } else {
            // mixed or inside stay same
          }
        }
      }
    } else {
      // Outside cell
      const labelList& cFaces = cells[cellI];
      FOR_ALL(cFaces, cFaceI) {
        const face& f = faces[cFaces[cFaceI]];
        FOR_ALL(f, fp) {
          label pointI = f[fp];
          if (pointSide[pointI] == NOTSET) {
            pointSide[pointI] = OUTSIDE;
          } else if (pointSide[pointI] == INSIDE) {
            pointSide[pointI] = MIXED;
          } else {
            // mixed or outside stay same
          }
        }
      }
    }
  }
  // Determine cells using mixed points only
  labelHashSet mixedOnlyCells{internalCells.size()};
  FOR_ALL_CONST_ITER(labelHashSet, internalCells, iter) {
    const label cellI = iter.key();
    const cell& cFaces = cells[cellI];
    label usesMixedOnly = true;
    FOR_ALL(cFaces, i) {
      const face& f = faces[cFaces[i]];
      FOR_ALL(f, fp) {
        if (pointSide[f[fp]] != MIXED) {
          usesMixedOnly = false;
          break;
        }
      }
      if (!usesMixedOnly) {
        break;
      }
    }
    if (usesMixedOnly) {
      mixedOnlyCells.insert(cellI);
    }
  }
  return mixedOnlyCells;
}

