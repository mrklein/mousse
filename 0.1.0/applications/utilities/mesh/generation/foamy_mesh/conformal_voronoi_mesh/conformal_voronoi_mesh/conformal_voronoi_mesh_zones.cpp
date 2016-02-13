// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conformal_voronoi_mesh.hpp"
#include "poly_modify_face.hpp"
#include "poly_modify_cell.hpp"
#include "sync_tools.hpp"
#include "region_split.hpp"
#include "surface_zones_info.hpp"
// Member Functions 
void mousse::conformalVoronoiMesh::calcNeighbourCellCentres
(
  const polyMesh& mesh,
  const pointField& cellCentres,
  pointField& neiCc
) const
{
  label nBoundaryFaces = mesh.nFaces() - mesh.nInternalFaces();
  if (neiCc.size() != nBoundaryFaces)
  {
    FATAL_ERROR_IN("conformalVoronoiMesh::calcNeighbourCellCentres(..)")
      << "nBoundaries:" << nBoundaryFaces
      << " neiCc:" << neiCc.size()
      << abort(FatalError);
  }
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    const labelUList& faceCells = pp.faceCells();
    label bFaceI = pp.start() - mesh.nInternalFaces();
    if (pp.coupled())
    {
      FOR_ALL(faceCells, i)
      {
        neiCc[bFaceI] = cellCentres[faceCells[i]];
        bFaceI++;
      }
    }
  }
  // Swap coupled boundaries. Apply separation to cc since is coordinate.
  syncTools::swapBoundaryFacePositions(mesh, neiCc);
}
void mousse::conformalVoronoiMesh::selectSeparatedCoupledFaces
(
  const polyMesh& mesh,
  boolList& selected
) const
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    // Check all coupled. Avoid using .coupled() so we also pick up AMI.
    if (isA<coupledPolyPatch>(patches[patchI]))
    {
      const coupledPolyPatch& cpp = refCast<const coupledPolyPatch>
      (
        patches[patchI]
      );
      if (cpp.separated() || !cpp.parallel())
      {
        FOR_ALL(cpp, i)
        {
          selected[cpp.start()+i] = true;
        }
      }
    }
  }
}
void mousse::conformalVoronoiMesh::findCellZoneInsideWalk
(
  const polyMesh& mesh,
  const labelList& locationSurfaces,  // indices of surfaces with inside point
  const labelList& faceToSurface, // per face index of named surface
  labelList& cellToSurface
) const
{
  // Analyse regions. Reuse regionsplit
  boolList blockedFace(mesh.nFaces());
  selectSeparatedCoupledFaces(mesh, blockedFace);
  FOR_ALL(faceToSurface, faceI)
  {
    if (faceToSurface[faceI] == -1)
    {
      blockedFace[faceI] = false;
    }
    else
    {
      blockedFace[faceI] = true;
    }
  }
  // No need to sync since namedSurfaceIndex already is synced
  // Set region per cell based on walking
  regionSplit cellRegion(mesh, blockedFace);
  blockedFace.clear();
  // Force calculation of face decomposition (used in findCell)
  (void)mesh.tetBasePtIs();
  const PtrList<surfaceZonesInfo>& surfZones =
    geometryToConformTo().surfZones();
  // For all locationSurface find the cell
  FOR_ALL(locationSurfaces, i)
  {
    label surfI = locationSurfaces[i];
    const mousse::point& insidePoint = surfZones[surfI].zoneInsidePoint();
    const word& surfName = geometryToConformTo().geometry().names()[surfI];
    Info<< "    For surface " << surfName
      << " finding inside point " << insidePoint
      << endl;
    // Find the region containing the insidePoint
    label keepRegionI = -1;
    label cellI = mesh.findCell(insidePoint);
    if (cellI != -1)
    {
      keepRegionI = cellRegion[cellI];
    }
    reduce(keepRegionI, maxOp<label>());
    Info<< "    For surface " << surfName
      << " found point " << insidePoint << " in cell " << cellI
      << " in global region " << keepRegionI
      << " out of " << cellRegion.nRegions() << " regions." << endl;
    if (keepRegionI == -1)
    {
      FATAL_ERROR_IN
      (
        "conformalVoronoiMesh::findCellZoneInsideWalk"
        "(const polyMesh&, const labelList&"
        ", const labelList&, labelList&)"
      )   << "Point " << insidePoint
        << " is not inside the mesh." << nl
        << "Bounding box of the mesh:" << mesh.bounds()
        << exit(FatalError);
    }
    // Set all cells with this region
    FOR_ALL(cellRegion, cellI)
    {
      if (cellRegion[cellI] == keepRegionI)
      {
        if (cellToSurface[cellI] == -2)
        {
          cellToSurface[cellI] = surfI;
        }
        else if (cellToSurface[cellI] != surfI)
        {
          WARNING_IN
          (
            "conformalVoronoiMesh::findCellZoneInsideWalk"
            "(const labelList&, const labelList&"
            ", const labelList&, const labelList&)"
          )   << "Cell " << cellI
            << " at " << mesh.cellCentres()[cellI]
            << " is inside surface " << surfName
            << " but already marked as being in zone "
            << cellToSurface[cellI] << endl
            << "This can happen if your surfaces are not"
            << " (sufficiently) closed."
            << endl;
        }
      }
    }
  }
}
mousse::labelList mousse::conformalVoronoiMesh::calcCellZones
(
  const pointField& cellCentres
) const
{
  labelList cellToSurface(cellCentres.size(), label(-1));
  const PtrList<surfaceZonesInfo>& surfZones =
    geometryToConformTo().surfZones();
  // Get list of closed surfaces
  labelList closedNamedSurfaces
  (
    surfaceZonesInfo::getAllClosedNamedSurfaces
    (
      surfZones,
      geometryToConformTo().geometry(),
      geometryToConformTo().surfaces()
    )
  );
  FOR_ALL(closedNamedSurfaces, i)
  {
    label surfI = closedNamedSurfaces[i];
    const searchableSurface& surface =
      allGeometry()[geometryToConformTo().surfaces()[surfI]];
    const surfaceZonesInfo::areaSelectionAlgo selectionMethod =
      surfZones[surfI].zoneInside();
    if
    (
      selectionMethod != surfaceZonesInfo::INSIDE
    && selectionMethod != surfaceZonesInfo::OUTSIDE
    && selectionMethod != surfaceZonesInfo::INSIDEPOINT
    )
    {
      FATAL_ERROR_IN("conformalVoronoiMesh::calcCellZones(..)")
        << "Trying to use surface "
        << surface.name()
        << " which has non-geometric inside selection method "
        << surfaceZonesInfo::areaSelectionAlgoNames[selectionMethod]
        << exit(FatalError);
    }
    if (surface.hasVolumeType())
    {
      List<volumeType> volType;
      surface.getVolumeType(cellCentres, volType);
      bool selectInside = true;
      if (selectionMethod == surfaceZonesInfo::INSIDEPOINT)
      {
        List<volumeType> volTypeInsidePoint;
        surface.getVolumeType
        (
          pointField(1, surfZones[surfI].zoneInsidePoint()),
          volTypeInsidePoint
        );
        if (volTypeInsidePoint[0] == volumeType::OUTSIDE)
        {
          selectInside = false;
        }
      }
      else if (selectionMethod == surfaceZonesInfo::OUTSIDE)
      {
        selectInside = false;
      }
      FOR_ALL(volType, pointI)
      {
        if (cellToSurface[pointI] == -1)
        {
          if
          (
            (
              volType[pointI] == volumeType::INSIDE
            && selectInside
            )
          || (
              volType[pointI] == volumeType::OUTSIDE
            && !selectInside
            )
          )
          {
            cellToSurface[pointI] = surfI;
          }
        }
      }
    }
  }
  return cellToSurface;
}
void mousse::conformalVoronoiMesh::calcFaceZones
(
  const polyMesh& mesh,
  const pointField& cellCentres,
  const labelList& cellToSurface,
  labelList& faceToSurface,
  boolList& flipMap
) const
{
  faceToSurface.setSize(mesh.nFaces(), -1);
  flipMap.setSize(mesh.nFaces(), false);
  const faceList& faces = mesh.faces();
  const labelList& faceOwner = mesh.faceOwner();
  const labelList& faceNeighbour = mesh.faceNeighbour();
  labelList neiFaceOwner(mesh.nFaces() - mesh.nInternalFaces(), label(-1));
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    const labelUList& faceCells = pp.faceCells();
    label bFaceI = pp.start() - mesh.nInternalFaces();
    if (pp.coupled())
    {
      FOR_ALL(faceCells, i)
      {
        neiFaceOwner[bFaceI] = cellToSurface[faceCells[i]];
        bFaceI++;
      }
    }
  }
  syncTools::swapBoundaryFaceList(mesh, neiFaceOwner);
  FOR_ALL(faces, faceI)
  {
    const label ownerSurfaceI = cellToSurface[faceOwner[faceI]];
    if (faceToSurface[faceI] >= 0)
    {
      continue;
    }
    if (mesh.isInternalFace(faceI))
    {
      const label neiSurfaceI = cellToSurface[faceNeighbour[faceI]];
      if
      (
        (ownerSurfaceI >= 0 || neiSurfaceI >= 0)
      && ownerSurfaceI != neiSurfaceI
      )
      {
        flipMap[faceI] =
          (
            ownerSurfaceI == max(ownerSurfaceI, neiSurfaceI)
           ? false
           : true
          );
        faceToSurface[faceI] = max(ownerSurfaceI, neiSurfaceI);
      }
    }
    else
    {
      label patchID = mesh.boundaryMesh().whichPatch(faceI);
      if (mesh.boundaryMesh()[patchID].coupled())
      {
        const label neiSurfaceI =
          neiFaceOwner[faceI - mesh.nInternalFaces()];
        if
        (
          (ownerSurfaceI >= 0 || neiSurfaceI >= 0)
        && ownerSurfaceI != neiSurfaceI
        )
        {
          flipMap[faceI] =
            (
              ownerSurfaceI == max(ownerSurfaceI, neiSurfaceI)
             ? false
             : true
            );
          faceToSurface[faceI] = max(ownerSurfaceI, neiSurfaceI);
        }
      }
      else
      {
        if (ownerSurfaceI >= 0)
        {
          faceToSurface[faceI] = ownerSurfaceI;
        }
      }
    }
  }
  const PtrList<surfaceZonesInfo>& surfZones =
    geometryToConformTo().surfZones();
  labelList unclosedSurfaces
  (
    surfaceZonesInfo::getUnclosedNamedSurfaces
    (
      surfZones,
      geometryToConformTo().geometry(),
      geometryToConformTo().surfaces()
    )
  );
  pointField neiCc(mesh.nFaces() - mesh.nInternalFaces());
  calcNeighbourCellCentres
  (
    mesh,
    cellCentres,
    neiCc
  );
  // Use intersection of cellCentre connections
  FOR_ALL(faces, faceI)
  {
    if (faceToSurface[faceI] >= 0)
    {
      continue;
    }
    label patchID = mesh.boundaryMesh().whichPatch(faceI);
    const label own = faceOwner[faceI];
    List<pointIndexHit> surfHit;
    labelList hitSurface;
    if (mesh.isInternalFace(faceI))
    {
      const label nei = faceNeighbour[faceI];
      geometryToConformTo().findSurfaceAllIntersections
      (
        cellCentres[own],
        cellCentres[nei],
        surfHit,
        hitSurface
      );
    }
    else if (patchID != -1 && mesh.boundaryMesh()[patchID].coupled())
    {
      geometryToConformTo().findSurfaceAllIntersections
      (
        cellCentres[own],
        neiCc[faceI - mesh.nInternalFaces()],
        surfHit,
        hitSurface
      );
    }
    // If there are multiple intersections then do not add to
    // a faceZone
    if (surfHit.size() == 1 && surfHit[0].hit())
    {
      if (findIndex(unclosedSurfaces, hitSurface[0]) != -1)
      {
        vectorField norm;
        geometryToConformTo().getNormal
        (
          hitSurface[0],
          List<pointIndexHit>(1, surfHit[0]),
          norm
        );
        vector fN = faces[faceI].normal(mesh.points());
        fN /= mag(fN) + SMALL;
        if ((norm[0] & fN) < 0)
        {
          flipMap[faceI] = true;
        }
        else
        {
          flipMap[faceI] = false;
        }
        faceToSurface[faceI] = hitSurface[0];
      }
    }
  }
//    labelList neiCellSurface(mesh.nFaces()-mesh.nInternalFaces());
//
//    FOR_ALL(patches, patchI)
//    {
//        const polyPatch& pp = patches[patchI];
//
//        if (pp.coupled())
//        {
//            FOR_ALL(pp, i)
//            {
//                label faceI = pp.start()+i;
//                label ownSurface = cellToSurface[faceOwner[faceI]];
//                neiCellSurface[faceI - mesh.nInternalFaces()] = ownSurface;
//            }
//        }
//    }
//    syncTools::swapBoundaryFaceList(mesh, neiCellSurface);
//
//    FOR_ALL(patches, patchI)
//    {
//        const polyPatch& pp = patches[patchI];
//
//        if (pp.coupled())
//        {
//            FOR_ALL(pp, i)
//            {
//                label faceI = pp.start()+i;
//                label ownSurface = cellToSurface[faceOwner[faceI]];
//                label neiSurface =
//                    neiCellSurface[faceI-mesh.nInternalFaces()];
//
//                if (faceToSurface[faceI] == -1 && (ownSurface != neiSurface))
//                {
//                    // Give face the max cell zone
//                    faceToSurface[faceI] =  max(ownSurface, neiSurface);
//                }
//            }
//        }
//    }
  // Sync
  syncTools::syncFaceList(mesh, faceToSurface, maxEqOp<label>());
}
void mousse::conformalVoronoiMesh::addZones
(
  polyMesh& mesh,
  const pointField& cellCentres
) const
{
  Info<< "    Adding zones to mesh" << endl;
  const PtrList<surfaceZonesInfo>& surfZones =
    geometryToConformTo().surfZones();
  labelList cellToSurface(calcCellZones(cellCentres));
  labelList faceToSurface;
  boolList flipMap;
  calcFaceZones
  (
    mesh,
    cellCentres,
    cellToSurface,
    faceToSurface,
    flipMap
  );
  labelList insidePointNamedSurfaces
  (
    surfaceZonesInfo::getInsidePointNamedSurfaces(surfZones)
  );
  findCellZoneInsideWalk
  (
    mesh,
    insidePointNamedSurfaces,
    faceToSurface,
    cellToSurface
  );
  labelList namedSurfaces(surfaceZonesInfo::getNamedSurfaces(surfZones));
  FOR_ALL(namedSurfaces, i)
  {
    label surfI = namedSurfaces[i];
    Info<< incrIndent << indent << "Surface : "
      << geometryToConformTo().geometry().names()[surfI] << nl
      << indent << "    faceZone : "
      << surfZones[surfI].faceZoneName() << nl
      << indent << "    cellZone : "
      << surfZones[surfI].cellZoneName()
      << decrIndent << endl;
  }
  // Add zones to mesh
  labelList surfaceToFaceZone =
    surfaceZonesInfo::addFaceZonesToMesh
    (
      surfZones,
      namedSurfaces,
      mesh
    );
  labelList surfaceToCellZone =
    surfaceZonesInfo::addCellZonesToMesh
    (
      surfZones,
      namedSurfaces,
      mesh
    );
  // Topochange container
  polyTopoChange meshMod(mesh);
  FOR_ALL(cellToSurface, cellI)
  {
    label surfaceI = cellToSurface[cellI];
    if (surfaceI >= 0)
    {
      label zoneI = surfaceToCellZone[surfaceI];
      if (zoneI >= 0)
      {
        meshMod.setAction
        (
          polyModifyCell
          (
            cellI,
            false,          // removeFromZone
            zoneI
          )
        );
      }
    }
  }
  const labelList& faceOwner = mesh.faceOwner();
  const labelList& faceNeighbour = mesh.faceNeighbour();
  FOR_ALL(faceToSurface, faceI)
  {
    label surfaceI = faceToSurface[faceI];
    if (surfaceI < 0)
    {
      continue;
    }
    label patchID = mesh.boundaryMesh().whichPatch(faceI);
    if (mesh.isInternalFace(faceI))
    {
      label own = faceOwner[faceI];
      label nei = faceNeighbour[faceI];
      meshMod.setAction
      (
        polyModifyFace
        (
          mesh.faces()[faceI],            // modified face
          faceI,                          // label of face
          own,                            // owner
          nei,                            // neighbour
          false,                          // face flip
          -1,                             // patch for face
          false,                          // remove from zone
          surfaceToFaceZone[surfaceI],    // zone for face
          flipMap[faceI]                  // face flip in zone
        )
      );
    }
    else if (patchID != -1 && mesh.boundaryMesh()[patchID].coupled())
    {
      label own = faceOwner[faceI];
      meshMod.setAction
      (
        polyModifyFace
        (
          mesh.faces()[faceI],            // modified face
          faceI,                          // label of face
          own,                            // owner
          -1,                             // neighbour
          false,                          // face flip
          patchID,                        // patch for face
          false,                          // remove from zone
          surfaceToFaceZone[surfaceI],    // zone for face
          flipMap[faceI]                  // face flip in zone
        )
      );
    }
  }
  // Change the mesh (no inflation, parallel sync)
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false, true);
}
