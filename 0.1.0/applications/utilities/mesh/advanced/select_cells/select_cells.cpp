// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "iodictionary.hpp"
#include "twod_point_corrector.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "tri_surface.hpp"
#include "tri_surface_search.hpp"
#include "mesh_search.hpp"
#include "cell_classification.hpp"
#include "cell_set.hpp"
#include "cell_info.hpp"
#include "mesh_wave.hpp"
#include "edge_stats.hpp"
#include "tree_data_tri_surface.hpp"
#include "indexed_octree.hpp"
#include "global_mesh_data.hpp"

using namespace mousse;
// cellType for cells included/not included in mesh.
static const label MESH = cellClassification::INSIDE;
static const label NONMESH = cellClassification::OUTSIDE;

void writeSet(const cellSet& cells, const string& msg)
{
  Info << "Writing " << msg << " (" << cells.size() << ") to cellSet "
    << cells.instance()/cells.local()/cells.name()
    << endl << endl;
  cells.write();
}
void getType(const labelList& elems, const label type, labelHashSet& set)
{
  FOR_ALL(elems, i)
  {
    if (elems[i] == type)
    {
      set.insert(i);
    }
  }
}
void cutBySurface
(
  const polyMesh& mesh,
  const meshSearch& queryMesh,
  const triSurfaceSearch& querySurf,
  const pointField& outsidePts,
  const bool selectCut,
  const bool selectInside,
  const bool selectOutside,
  const scalar nearDist,
  cellClassification& cellType
)
{
  // Cut with surface and classify as inside/outside/cut
  cellType =
    cellClassification
    (
      mesh,
      queryMesh,
      querySurf,
      outsidePts
    );
  // Get inside/outside/cutCells cellSets.
  cellSet inside{mesh, "inside", mesh.nCells()/10};
  getType(cellType, cellClassification::INSIDE, inside);
  writeSet(inside, "inside cells");
  cellSet outside{mesh, "outside", mesh.nCells()/10};
  getType(cellType, cellClassification::OUTSIDE, outside);
  writeSet(outside, "outside cells");
  cellSet cutCells{mesh, "cutCells", mesh.nCells()/10};
  getType(cellType, cellClassification::CUT, cutCells);
  writeSet(cutCells, "cells cut by surface");
  // Change cellType to reflect selected part of mesh. Use
  // MESH to denote selected part, NONMESH for all
  // other cells.
  // Is a bit of a hack but allows us to reuse all the functionality
  // in cellClassification.
  FOR_ALL(cellType, cellI)
  {
    label cType = cellType[cellI];
    if (cType == cellClassification::CUT)
    {
      if (selectCut)
      {
        cellType[cellI] = MESH;
      }
      else
      {
        cellType[cellI] = NONMESH;
      }
    }
    else if (cType == cellClassification::INSIDE)
    {
      if (selectInside)
      {
        cellType[cellI] = MESH;
      }
      else
      {
        cellType[cellI] = NONMESH;
      }
    }
    else if (cType == cellClassification::OUTSIDE)
    {
      if (selectOutside)
      {
        cellType[cellI] = MESH;
      }
      else
      {
        cellType[cellI] = NONMESH;
      }
    }
    else
    {
      FATAL_ERROR_IN("cutBySurface")
        << "Multiple mesh regions in original mesh" << endl
        << "Please use splitMeshRegions to separate these"
        << exit(FatalError);
    }
  }
  if (nearDist > 0)
  {
    Info << "Removing cells with points closer than " << nearDist
      << " to the surface ..." << nl << endl;
    const pointField& pts = mesh.points();
    const indexedOctree<treeDataTriSurface>& tree = querySurf.tree();
    label nRemoved = 0;
    FOR_ALL(pts, pointI)
    {
      const point& pt = pts[pointI];
      pointIndexHit hitInfo = tree.findNearest(pt, sqr(nearDist));
      if (hitInfo.hit())
      {
        const labelList& pCells = mesh.pointCells()[pointI];
        FOR_ALL(pCells, i)
        {
          if (cellType[pCells[i]] != NONMESH)
          {
            cellType[pCells[i]] = NONMESH;
            nRemoved++;
          }
        }
      }
    }
    Info << "Removed " << nRemoved << " cells since too close to surface"
      << nl << endl;
  }
}
// We're meshing the outside. Subset the currently selected mesh cells with the
// ones reachable from the outsidepoints.
label selectOutsideCells
(
  const polyMesh& mesh,
  const meshSearch& queryMesh,
  const pointField& outsidePts,
  cellClassification& cellType
)
{
  //
  // Check all outsidePts and for all of them inside a mesh cell
  // collect the faces to start walking from
  //
  // Outside faces
  labelHashSet outsideFacesMap{outsidePts.size() * 6 * 2};
  DynamicList<label> outsideFaces{outsideFacesMap.size()};
  // CellInfo on outside faces
  DynamicList<cellInfo> outsideFacesInfo{outsideFacesMap.size()};
  // cellInfo for mesh cell
  const cellInfo meshInfo{MESH};
  FOR_ALL(outsidePts, outsidePtI)
  {
    // Find cell containing point. Linear search.
    label cellI = queryMesh.findCell(outsidePts[outsidePtI], -1, false);
    if (cellI != -1 && cellType[cellI] == MESH)
    {
      Info << "Marking cell " << cellI << " containing outside point "
        << outsidePts[outsidePtI] << " with type " << cellType[cellI]
        << " ..." << endl;
      //
      // Mark this cell and its faces to start walking from
      //
      // Mark faces of cellI
      const labelList& cFaces = mesh.cells()[cellI];
      FOR_ALL(cFaces, i)
      {
        label faceI = cFaces[i];
        if (outsideFacesMap.insert(faceI))
        {
          outsideFaces.append(faceI);
          outsideFacesInfo.append(meshInfo);
        }
      }
    }
  }
  // Floodfill starting from outsideFaces (of type meshInfo)
  MeshWave<cellInfo> regionCalc
  {
    mesh,
    outsideFaces.shrink(),
    outsideFacesInfo.shrink(),
    mesh.globalData().nTotalCells()+1   // max iterations
  };
  // Now regionCalc should hold info on cells that are reachable from
  // changedFaces. Use these to subset cellType
  const List<cellInfo>& allCellInfo = regionCalc.allCellInfo();
  label nChanged = 0;
  FOR_ALL(allCellInfo, cellI)
  {
    if (cellType[cellI] == MESH)
    {
      // Original cell was selected for meshing. Check if cell was
      // reached from outsidePoints
      if (allCellInfo[cellI].type() != MESH)
      {
        cellType[cellI] = NONMESH;
        nChanged++;
      }
    }
  }
  return nChanged;
}
int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_poly_mesh.inc"
  // Mesh edge statistics calculator
  edgeStats edgeCalc{mesh};
  IOdictionary refineDict
  {
    {
      "selectCellsDict",
      runTime.system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  fileName surfName{refineDict.lookup("surface")};
  pointField outsidePts{refineDict.lookup("outsidePoints")};
  bool useSurface{readBool(refineDict.lookup("useSurface"))};
  bool selectCut{readBool(refineDict.lookup("selectCut"))};
  bool selectInside{readBool(refineDict.lookup("selectInside"))};
  bool selectOutside{readBool(refineDict.lookup("selectOutside"))};
  scalar nearDist{readScalar(refineDict.lookup("nearDistance"))};
  if (useSurface)
  {
    Info << "Cells to be used for meshing (0=false, 1=true):" << nl
      << "    cells cut by surface            : " << selectCut << nl
      << "    cells inside of surface         : " << selectInside << nl
      << "    cells outside of surface        : " << selectOutside << nl
      << "    cells with points further than  : " << nearDist << nl
      << endl;
  }
  else
  {
    Info << "Cells to be used for meshing (0=false, 1=true):" << nl
      << "    cells reachable from outsidePoints:" << selectOutside << nl
      << endl;
  }
  // Print edge stats on original mesh.
  (void)edgeCalc.minLen(Info);
  // Search engine on mesh. Face decomposition since faces might be warped.
  meshSearch queryMesh{mesh};
  // Check all 'outside' points
  FOR_ALL(outsidePts, outsideI)
  {
    const point& outsidePoint = outsidePts[outsideI];
    label cellI = queryMesh.findCell(outsidePoint, -1, false);
    if (returnReduce(cellI, maxOp<label>()) == -1)
    {
      FATAL_ERROR_IN(args.executable())
        << "outsidePoint " << outsidePoint
        << " is not inside any cell"
        << exit(FatalError);
    }
  }
  // Cell status (compared to surface if provided): inside/outside/cut.
  // Start off from everything selected and cut later.
  cellClassification cellType
  {
    mesh,
    {mesh.nCells(), cellClassification::MESH}
  };
  // Surface
  autoPtr<triSurface> surf{NULL};
  // Search engine on surface.
  autoPtr<triSurfaceSearch> querySurf{NULL};

  if (useSurface)
  {
    surf.reset(new triSurface(surfName));
    // Dump some stats
    surf().writeStats(Info);
    // Search engine on surface.
    querySurf.reset(new triSurfaceSearch(surf));
    // Set cellType[cellI] according to relation to surface
    cutBySurface
    (
      mesh,
      queryMesh,
      querySurf,
      outsidePts,
      selectCut,
      selectInside,
      selectOutside,
      nearDist,
      cellType
    );
  }
  // Now 'trim' all the corners from the mesh so meshing/surface extraction
  // becomes easier.
  label nHanging, nRegionEdges, nRegionPoints, nOutside;
  do
  {
    Info << "Removing cells which after subsetting would have all points"
      << " on outside ..." << nl << endl;
    nHanging = cellType.fillHangingCells
    (
      MESH,       // meshType
      NONMESH,    // fill type
      mesh.nCells()
    );
    Info << "Removing edges connecting cells unconnected by faces ..."
      << nl << endl;
    nRegionEdges = cellType.fillRegionEdges
    (
      MESH,       // meshType
      NONMESH,    // fill type
      mesh.nCells()
    );
    Info << "Removing points connecting cells unconnected by faces ..."
      << nl << endl;
    nRegionPoints = cellType.fillRegionPoints
    (
      MESH,       // meshType
      NONMESH,    // fill type
      mesh.nCells()
    );
    nOutside = 0;
    if (selectOutside)
    {
      // Since we're selecting the cells reachable from outsidePoints
      // and the set might have changed, redo the outsideCells
      // calculation
      nOutside = selectOutsideCells
      (
        mesh,
        queryMesh,
        outsidePts,
        cellType
      );
    }
  } while (nHanging != 0
           || nRegionEdges != 0
           || nRegionPoints != 0
           || nOutside != 0);
  cellSet selectedCells{mesh, "selected", mesh.nCells()/10};
  getType(cellType, MESH, selectedCells);
  writeSet(selectedCells, "cells selected for meshing");
  Info << "End\n" << endl;
  return 0;
}
