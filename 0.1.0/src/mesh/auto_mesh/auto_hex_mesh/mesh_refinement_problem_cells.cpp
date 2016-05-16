// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_refinement.hpp"
#include "fv_mesh.hpp"
#include "sync_tools.hpp"
#include "time.hpp"
#include "refinement_surfaces.hpp"
#include "point_set.hpp"
#include "face_set.hpp"
#include "indirect_primitive_patch.hpp"
#include "cell_set.hpp"
#include "searchable_surfaces.hpp"
#include "poly_mesh_geometry.hpp"
#include "iomanip.hpp"
#include "unit_conversion.hpp"
#include "auto_snap_driver.hpp"
#include "snap_parameters.hpp"
#include "motion_smoother.hpp"
#include "topo_distance_data.hpp"
#include "face_cell_wave.hpp"


// Private Member Functions 
void mousse::meshRefinement::markBoundaryFace
(
  const label faceI,
  boolList& isBoundaryFace,
  boolList& isBoundaryEdge,
  boolList& isBoundaryPoint
) const
{
  isBoundaryFace[faceI] = true;
  const labelList& fEdges = mesh_.faceEdges(faceI);
  FOR_ALL(fEdges, fp) {
    isBoundaryEdge[fEdges[fp]] = true;
  }
  const face& f = mesh_.faces()[faceI];
  FOR_ALL(f, fp) {
    isBoundaryPoint[f[fp]] = true;
  }
}


void mousse::meshRefinement::findNearest
(
  const labelList& meshFaces,
  List<pointIndexHit>& nearestInfo,
  labelList& nearestSurface,
  labelList& nearestRegion,
  vectorField& nearestNormal
) const
{
  pointField fc{meshFaces.size()};
  FOR_ALL(meshFaces, i) {
    fc[i] = mesh_.faceCentres()[meshFaces[i]];
  }
  const labelList allSurfaces{identity(surfaces_.surfaces().size())};
  surfaces_.findNearest
    (
      allSurfaces,
      fc,
      scalarField(fc.size(), sqr(GREAT)),    // sqr of attraction
      nearestSurface,
      nearestInfo
    );
  // Do normal testing per surface.
  nearestNormal.setSize(nearestInfo.size());
  nearestRegion.setSize(nearestInfo.size());
  FOR_ALL(allSurfaces, surfI) {
    DynamicList<pointIndexHit> localHits;
    FOR_ALL(nearestSurface, i) {
      if (nearestSurface[i] == surfI) {
        localHits.append(nearestInfo[i]);
      }
    }
    label geomI = surfaces_.surfaces()[surfI];
    pointField localNormals;
    surfaces_.geometry()[geomI].getNormal(localHits, localNormals);
    labelList localRegion;
    surfaces_.geometry()[geomI].getRegion(localHits, localRegion);
    label localI = 0;
    FOR_ALL(nearestSurface, i) {
      if (nearestSurface[i] == surfI) {
        nearestNormal[i] = localNormals[localI];
        nearestRegion[i] = localRegion[localI];
        localI++;
      }
    }
  }
}


mousse::Map<mousse::label> mousse::meshRefinement::findEdgeConnectedProblemCells
(
  const scalarField& perpendicularAngle,
  const labelList& globalToPatch
) const
{
  // Construct addressing engine from all patches added for meshing.
  autoPtr<indirectPrimitivePatch> ppPtr
  {
    meshRefinement::makePatch(mesh_, meshedPatches())
  };
  const indirectPrimitivePatch& pp = ppPtr();
  // 1. Collect faces to test
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  DynamicList<label> candidateFaces{pp.size()/20};
  const labelListList& edgeFaces = pp.edgeFaces();
  const labelList& cellLevel = meshCutter_.cellLevel();
  FOR_ALL(edgeFaces, edgeI) {
    const labelList& eFaces = edgeFaces[edgeI];
    if (eFaces.size() == 2) {
      label face0 = pp.addressing()[eFaces[0]];
      label face1 = pp.addressing()[eFaces[1]];
      label cell0 = mesh_.faceOwner()[face0];
      label cell1 = mesh_.faceOwner()[face1];
      if (cellLevel[cell0] > cellLevel[cell1]) {
        // cell0 smaller.
        const vector& n0 = pp.faceNormals()[eFaces[0]];
        const vector& n1 = pp.faceNormals()[eFaces[1]];
        if (mag(n0 & n1) < 0.1) {
          candidateFaces.append(face0);
        }
      } else if (cellLevel[cell1] > cellLevel[cell0]) {
        // cell1 smaller.
        const vector& n0 = pp.faceNormals()[eFaces[0]];
        const vector& n1 = pp.faceNormals()[eFaces[1]];
        if (mag(n0 & n1) < 0.1) {
          candidateFaces.append(face1);
        }
      }
    }
  }
  candidateFaces.shrink();
  Info << "Testing " << returnReduce(candidateFaces.size(), sumOp<label>())
    << " faces on edge-connected cells of differing level."
    << endl;
  if (debug&meshRefinement::MESH) {
    faceSet fSet{mesh_, "edgeConnectedFaces", candidateFaces};
    fSet.instance() = timeName();
    Pout << "Writing " << fSet.size()
      << " with problematic topology to faceSet "
      << fSet.objectPath() << endl;
    fSet.write();
  }
  // 2. Find nearest surface on candidate faces
  List<pointIndexHit> nearestInfo;
  labelList nearestSurface;
  labelList nearestRegion;
  vectorField nearestNormal;
  findNearest
    (
      candidateFaces,
      nearestInfo,
      nearestSurface,
      nearestRegion,
      nearestNormal
    );
  // 3. Test angle to surface
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  Map<label> candidateCells{candidateFaces.size()};
  faceSet perpFaces{mesh_, "perpendicularFaces", pp.size()/100};
  FOR_ALL(candidateFaces, i) {
    label faceI = candidateFaces[i];
    vector n = mesh_.faceAreas()[faceI];
    n /= mag(n);
    label region =
      surfaces_.globalRegion
      (
        nearestSurface[i],
        nearestRegion[i]
      );
    scalar angle = degToRad(perpendicularAngle[region]);
    if (angle >= 0) {
      if (mag(n & nearestNormal[i]) < mousse::sin(angle)) {
        perpFaces.insert(faceI);
        candidateCells.insert
          (
            mesh_.faceOwner()[faceI],
            globalToPatch[region]
          );
      }
    }
  }
  if (debug & meshRefinement::MESH) {
    perpFaces.instance() = timeName();
    Pout << "Writing " << perpFaces.size()
      << " faces that are perpendicular to the surface to set "
      << perpFaces.objectPath() << endl;
    perpFaces.write();
  }
  return candidateCells;
}


// Check if moving face to new points causes a 'collapsed' face.
// Uses new point position only for the face, not the neighbouring
// cell centres
bool mousse::meshRefinement::isCollapsedFace
(
  const pointField& points,
  const pointField& neiCc,
  const scalar minFaceArea,
  const scalar maxNonOrtho,
  const label faceI
) const
{
  // Severe nonorthogonality threshold
  const scalar severeNonorthogonalityThreshold = ::cos(degToRad(maxNonOrtho));
  vector s = mesh_.faces()[faceI].normal(points);
  scalar magS = mag(s);
  // Check face area
  if (magS < minFaceArea) {
    return true;
  }
  // Check orthogonality
  const point& ownCc = mesh_.cellCentres()[mesh_.faceOwner()[faceI]];
  if (mesh_.isInternalFace(faceI)) {
    label nei = mesh_.faceNeighbour()[faceI];
    vector d = mesh_.cellCentres()[nei] - ownCc;
    scalar dDotS = (d & s)/(mag(d)*magS + VSMALL);
    return (dDotS < severeNonorthogonalityThreshold);
  } else {
    label patchI = mesh_.boundaryMesh().whichPatch(faceI);
    if (mesh_.boundaryMesh()[patchI].coupled()) {
      vector d = neiCc[faceI-mesh_.nInternalFaces()] - ownCc;
      scalar dDotS = (d & s)/(mag(d)*magS + VSMALL);
      return (dDotS < severeNonorthogonalityThreshold);
    } else {
      // Collapsing normal boundary face does not cause problems with
      // non-orthogonality
      return false;
    }
  }
}


// Check if moving cell to new points causes it to collapse.
bool mousse::meshRefinement::isCollapsedCell
(
  const pointField& points,
  const scalar volFraction,
  const label cellI
) const
{
  scalar vol = mesh_.cells()[cellI].mag(points, mesh_.faces());
  return vol/mesh_.cellVolumes()[cellI] < volFraction;
}


mousse::labelList mousse::meshRefinement::nearestPatch
(
  const labelList& adaptPatchIDs
) const
{
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  labelList nearestAdaptPatch;
  if (adaptPatchIDs.size()) {
    nearestAdaptPatch.setSize(mesh_.nFaces(), adaptPatchIDs[0]);
    // Count number of faces in adaptPatchIDs
    label nFaces = 0;
    FOR_ALL(adaptPatchIDs, i) {
      const polyPatch& pp = patches[adaptPatchIDs[i]];
      nFaces += pp.size();
    }
    // Field on cells and faces.
    List<topoDistanceData> cellData{mesh_.nCells()};
    List<topoDistanceData> faceData{mesh_.nFaces()};
    // Start of changes
    labelList patchFaces{nFaces};
    List<topoDistanceData> patchData{nFaces};
    nFaces = 0;
    FOR_ALL(adaptPatchIDs, i) {
      label patchI = adaptPatchIDs[i];
      const polyPatch& pp = patches[patchI];
      FOR_ALL(pp, i) {
        patchFaces[nFaces] = pp.start()+i;
        patchData[nFaces] = topoDistanceData(patchI, 0);
        nFaces++;
      }
    }
    // Propagate information inwards
    FaceCellWave<topoDistanceData> deltaCalc
    {
      mesh_,
      patchFaces,
      patchData,
      faceData,
      cellData,
      mesh_.globalData().nTotalCells()+1
    };
    // And extract
    bool haveWarned = false;
    FOR_ALL(faceData, faceI) {
      if (!faceData[faceI].valid(deltaCalc.data())) {
        if (!haveWarned) {
          WARNING_IN("meshRefinement::nearestPatch(..)")
            << "Did not visit some faces, e.g. face " << faceI
            << " at " << mesh_.faceCentres()[faceI] << endl
            << "Assigning  these cells to patch "
            << adaptPatchIDs[0]
            << endl;
          haveWarned = true;
        }
      } else {
        nearestAdaptPatch[faceI] = faceData[faceI].data();
      }
    }
  } else {
    // Use patch 0
    nearestAdaptPatch.setSize(mesh_.nFaces(), 0);
  }
  return nearestAdaptPatch;
}


// Returns list with for every internal face -1 or the patch they should
// be baffled into. Gets run after createBaffles so all the unzoned surface
// intersections have already been turned into baffles. (Note: zoned surfaces
// are not baffled at this stage)
// Used to remove cells by baffling all their faces and have the
// splitMeshRegions chuck away non used regions.
mousse::labelList mousse::meshRefinement::markFacesOnProblemCells
(
  const dictionary& motionDict,
  const bool removeEdgeConnectedCells,
  const scalarField& perpendicularAngle,
  const labelList& globalToPatch
) const
{
  const labelList& cellLevel = meshCutter_.cellLevel();
  const labelList& pointLevel = meshCutter_.pointLevel();
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  // Mark all points and edges on baffle patches (so not on any inlets,
  // outlets etc.)
  boolList isBoundaryPoint{mesh_.nPoints(), false};
  boolList isBoundaryEdge{mesh_.nEdges(), false};
  boolList isBoundaryFace{mesh_.nFaces(), false};
  // Fill boundary data. All elements on meshed patches get marked.
  // Get the labels of added patches.
  labelList adaptPatchIDs{meshedPatches()};
  FOR_ALL(adaptPatchIDs, i) {
    const polyPatch& pp = patches[adaptPatchIDs[i]];
    label faceI = pp.start();
    FOR_ALL(pp, j) {
      markBoundaryFace
        (
          faceI,
          isBoundaryFace,
          isBoundaryEdge,
          isBoundaryPoint
        );
      faceI++;
    }
  }
  // Per face the nearest adaptPatch
  const labelList nearestAdaptPatch{nearestPatch(adaptPatchIDs)};
  // Per internal face (boundary faces not used) the patch that the
  // baffle should get (or -1)
  labelList facePatch{mesh_.nFaces(), -1};
  // Swap neighbouring cell centres and cell level
  labelList neiLevel{mesh_.nFaces()-mesh_.nInternalFaces()};
  pointField neiCc{mesh_.nFaces()-mesh_.nInternalFaces()};
  calcNeighbourData(neiLevel, neiCc);
  // Count of faces marked for baffling
  label nBaffleFaces = 0;
  PackedBoolList isMasterFace{syncTools::getMasterFaces(mesh_)};
  // Count of faces not baffled since would not cause a collapse
  label nPrevented = 0;
  if (removeEdgeConnectedCells && max(perpendicularAngle) >= 0) {
    Info << "markFacesOnProblemCells :"
      << " Checking for edge-connected cells of highly differing sizes."
      << endl;
    // Pick up the cells that need to be removed and (a guess for)
    // the patch they should be patched with.
    Map<label> problemCells
    {
      findEdgeConnectedProblemCells(perpendicularAngle, globalToPatch)
    };
    // Baffle all faces of cells that need to be removed
    FOR_ALL_CONST_ITER(Map<label>, problemCells, iter) {
      const cell& cFaces = mesh_.cells()[iter.key()];
      FOR_ALL(cFaces, i) {
        label faceI = cFaces[i];
        if (facePatch[faceI] != -1 || !mesh_.isInternalFace(faceI))
          continue;
        facePatch[faceI] = nearestAdaptPatch[faceI];
        nBaffleFaces++;
        // Mark face as a 'boundary'
        markBoundaryFace
          (
            faceI,
            isBoundaryFace,
            isBoundaryEdge,
            isBoundaryPoint
          );
      }
    }
    Info << "markFacesOnProblemCells : Marked "
      << returnReduce(nBaffleFaces, sumOp<label>())
      << " additional internal faces to be converted into baffles"
      << " due to "
      << returnReduce(problemCells.size(), sumOp<label>())
      << " cells edge-connected to lower level cells." << endl;
    if (debug&meshRefinement::MESH) {
      cellSet problemCellSet(mesh_, "problemCells", problemCells.toc());
      problemCellSet.instance() = timeName();
      Pout << "Writing " << problemCellSet.size()
        << " cells that are edge connected to coarser cell to set "
        << problemCellSet.objectPath() << endl;
      problemCellSet.write();
    }
  }
  syncTools::syncPointList
    (
      mesh_,
      isBoundaryPoint,
      orEqOp<bool>(),
      false               // null value
    );
  syncTools::syncEdgeList
    (
      mesh_,
      isBoundaryEdge,
      orEqOp<bool>(),
      false               // null value
    );
  syncTools::syncFaceList
    (
      mesh_,
      isBoundaryFace,
      orEqOp<bool>()
    );
  // See if checking for collapse
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Collapse checking parameters
  const scalar volFraction =
    motionDict.lookupOrDefault<scalar>("minVolCollapseRatio", -1);
  const bool checkCollapse = (volFraction > 0);
  scalar minArea = -1;
  scalar maxNonOrtho = -1;
  // Find nearest (non-baffle) surface
  pointField newPoints;
  if (checkCollapse) {
    minArea = readScalar(motionDict.lookup("minArea"));
    maxNonOrtho = readScalar(motionDict.lookup("maxNonOrtho"));
    Info << "markFacesOnProblemCells :"
      << " Deleting all-anchor surface cells only if"
      << " snapping them violates mesh quality constraints:" << nl
      << "    snapped/original cell volume < " << volFraction << nl
      << "    face area                    < " << minArea << nl
      << "    non-orthogonality            > " << maxNonOrtho << nl
      << endl;
    // Construct addressing engine.
    autoPtr<indirectPrimitivePatch> ppPtr
    {
      meshRefinement::makePatch(mesh_, adaptPatchIDs)
    };
    const indirectPrimitivePatch& pp = ppPtr();
    const pointField& localPoints = pp.localPoints();
    const labelList& meshPoints = pp.meshPoints();
    List<pointIndexHit> hitInfo;
    labelList hitSurface;
    surfaces_.findNearest
      (
        surfaceZonesInfo::getUnnamedSurfaces(surfaces_.surfZones()),
        localPoints,
        scalarField(localPoints.size(), sqr(GREAT)),    // sqr of attraction
        hitSurface,
        hitInfo
      );
    // Start off from current points
    newPoints = mesh_.points();
    FOR_ALL(hitInfo, i) {
      if (hitInfo[i].hit()) {
        newPoints[meshPoints[i]] = hitInfo[i].hitPoint();
      }
    }
    if (debug & meshRefinement::MESH) {
      const_cast<Time&>(mesh_.time())++;
      pointField oldPoints{mesh_.points()};
      mesh_.movePoints(newPoints);
      Pout << "Writing newPoints mesh to time " << timeName() << endl;
      write
        (
          debugType(debug),
          writeType(writeLevel() | WRITEMESH),
          mesh_.time().path()/"newPoints"
        );
      mesh_.movePoints(oldPoints);
    }
  }
  // For each cell count the number of anchor points that are on
  // the boundary:
  // 8 : check the number of (baffle) boundary faces. If 3 or more block
  //     off the cell since the cell would get squeezed down to a diamond
  //     (probably; if the 3 or more faces are unrefined (only use the
  //      anchor points))
  // 7 : store. Used to check later on whether there are points with
  //     3 or more of these cells. (note that on a flat surface a boundary
  //     point will only have 4 cells connected to it)
  // Does cell have exactly 7 of its 8 anchor points on the boundary?
  PackedBoolList hasSevenBoundaryAnchorPoints{mesh_.nCells()};
  // If so what is the remaining non-boundary anchor point?
  labelHashSet nonBoundaryAnchors{mesh_.nCells()/10000};
  // On-the-fly addressing storage.
  DynamicList<label> dynFEdges;
  DynamicList<label> dynCPoints;
  FOR_ALL(cellLevel, cellI) {
    const labelList& cPoints = mesh_.cellPoints(cellI, dynCPoints);
    // Get number of anchor points (pointLevel <= cellLevel)
    label nBoundaryAnchors = 0;
    label nNonAnchorBoundary = 0;
    label nonBoundaryAnchor = -1;
    FOR_ALL(cPoints, i) {
      label pointI = cPoints[i];
      if (pointLevel[pointI] <= cellLevel[cellI]) {
        // Anchor point
        if (isBoundaryPoint[pointI]) {
          nBoundaryAnchors++;
        } else {
          // Anchor point which is not on the surface
          nonBoundaryAnchor = pointI;
        }
      } else if (isBoundaryPoint[pointI]) {
        nNonAnchorBoundary++;
      }
    }
    if (nBoundaryAnchors == 8) {
      const cell& cFaces = mesh_.cells()[cellI];
      // Count boundary faces.
      label nBfaces = 0;
      FOR_ALL(cFaces, cFaceI) {
        if (isBoundaryFace[cFaces[cFaceI]]) {
          nBfaces++;
        }
      }
      // If nBfaces > 1 make all non-boundary non-baffle faces baffles.
      // We assume that this situation is where there is a single
      // cell sticking out which would get flattened.
      // Eugene: delete cell no matter what.
      //if (nBfaces > 1)
      {
        if (checkCollapse && !isCollapsedCell(newPoints, volFraction, cellI)) {
          nPrevented++;
        } else {
          // Block all faces of cell
          FOR_ALL(cFaces, cf) {
            label faceI = cFaces[cf];
            if (facePatch[faceI] == -1 && mesh_.isInternalFace(faceI)) {
              facePatch[faceI] = nearestAdaptPatch[faceI];
              nBaffleFaces++;
              // Mark face as a 'boundary'
              markBoundaryFace
                (
                  faceI,
                  isBoundaryFace,
                  isBoundaryEdge,
                  isBoundaryPoint
                );
            }
          }
        }
      }
    } else if (nBoundaryAnchors == 7) {
      // Mark the cell. Store the (single!) non-boundary anchor point.
      hasSevenBoundaryAnchorPoints.set(cellI, 1u);
      nonBoundaryAnchors.insert(nonBoundaryAnchor);
    }
  }
  // Loop over all points. If a point is connected to 4 or more cells
  // with 7 anchor points on the boundary set those cell's non-boundary faces
  // to baffles
  DynamicList<label> dynPCells;
  FOR_ALL_CONST_ITER(labelHashSet, nonBoundaryAnchors, iter) {
    label pointI = iter.key();
    const labelList& pCells = mesh_.pointCells(pointI, dynPCells);
    // Count number of 'hasSevenBoundaryAnchorPoints' cells.
    label n = 0;
    FOR_ALL(pCells, i) {
      if (hasSevenBoundaryAnchorPoints.get(pCells[i]) == 1u) {
        n++;
      }
    }
    if (n > 3) {
      // Point in danger of being what? Remove all 7-cells.
      FOR_ALL(pCells, i) {
        label cellI = pCells[i];
        if (hasSevenBoundaryAnchorPoints.get(cellI) == 1u) {
          if (checkCollapse && !isCollapsedCell(newPoints, volFraction, cellI)) {
            nPrevented++;
          } else {
            const cell& cFaces = mesh_.cells()[cellI];
            FOR_ALL(cFaces, cf) {
              label faceI = cFaces[cf];
              if (facePatch[faceI] != -1 || !mesh_.isInternalFace(faceI))
                continue;
              facePatch[faceI] = nearestAdaptPatch[faceI];
              nBaffleFaces++;
              // Mark face as a 'boundary'
              markBoundaryFace
                (
                  faceI,
                  isBoundaryFace,
                  isBoundaryEdge,
                  isBoundaryPoint
                );
            }
          }
        }
      }
    }
  }
  // Sync all. (note that pointdata and facedata not used anymore but sync
  // anyway)
  syncTools::syncPointList
    (
      mesh_,
      isBoundaryPoint,
      orEqOp<bool>(),
      false               // null value
    );
  syncTools::syncEdgeList
    (
      mesh_,
      isBoundaryEdge,
      orEqOp<bool>(),
      false               // null value
    );
  syncTools::syncFaceList
    (
      mesh_,
      isBoundaryFace,
      orEqOp<bool>()
    );
  // Find faces with all edges on the boundary and make them baffles
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++) {
    if (facePatch[faceI] != -1)
      continue;
    const labelList& fEdges = mesh_.faceEdges(faceI, dynFEdges);
    label nFaceBoundaryEdges = 0;
    FOR_ALL(fEdges, fe) {
      if (isBoundaryEdge[fEdges[fe]]) {
        nFaceBoundaryEdges++;
      }
    }
    if (nFaceBoundaryEdges != fEdges.size())
      continue;
    if (checkCollapse && !isCollapsedFace(newPoints, neiCc, minArea,
                                          maxNonOrtho, faceI)) {
      nPrevented++;
    } else {
      facePatch[faceI] = nearestAdaptPatch[faceI];
      nBaffleFaces++;
      // Do NOT update boundary data since this would grow blocked
      // faces across gaps.
    }
  }
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    if (!pp.coupled())
      continue;
    label faceI = pp.start();
    FOR_ALL(pp, i) {
      if (facePatch[faceI] == -1) {
        const labelList& fEdges = mesh_.faceEdges(faceI, dynFEdges);
        label nFaceBoundaryEdges = 0;
        FOR_ALL(fEdges, fe) {
          if (isBoundaryEdge[fEdges[fe]]) {
            nFaceBoundaryEdges++;
          }
        }
        if (nFaceBoundaryEdges == fEdges.size()) {
          if (checkCollapse && !isCollapsedFace(newPoints, neiCc, minArea,
                                                maxNonOrtho, faceI)) {
            nPrevented++;
          } else {
            facePatch[faceI] = nearestAdaptPatch[faceI];
            if (isMasterFace[faceI]) {
              nBaffleFaces++;
            }
            // Do NOT update boundary data since this would grow
            // blocked faces across gaps.
          }
        }
      }
      faceI++;
    }
  }
  // Because of isCollapsedFace one side can decide not to baffle whereas
  // the other side does so sync. Baffling is prefered over not baffling.
  if (checkCollapse) {  // Or always?
    syncTools::syncFaceList
      (
        mesh_,
        facePatch,
        maxEqOp<label>()
      );
  }
  Info << "markFacesOnProblemCells : marked "
    << returnReduce(nBaffleFaces, sumOp<label>())
    << " additional internal faces to be converted into baffles."
    << endl;
  if (checkCollapse) {
    Info << "markFacesOnProblemCells : prevented "
      << returnReduce(nPrevented, sumOp<label>())
      << " internal faces from getting converted into baffles."
      << endl;
  }
  return facePatch;
}


// Mark faces to be baffled to prevent snapping problems. Does
// test to find nearest surface and checks which faces would get squashed.
mousse::labelList mousse::meshRefinement::markFacesOnProblemCellsGeometric
(
  const snapParameters& snapParams,
  const dictionary& motionDict
) const
{
  pointField oldPoints{mesh_.points()};
  // Repeat (most of) autoSnapDriver::doSnap
  {
    labelList adaptPatchIDs{meshedPatches()};
    // Construct addressing engine.
    autoPtr<indirectPrimitivePatch> ppPtr
    {
      meshRefinement::makePatch(mesh_, adaptPatchIDs)
    };
    indirectPrimitivePatch& pp = ppPtr();
    // Distance to attract to nearest feature on surface
    const scalarField snapDist
    {
      autoSnapDriver::calcSnapDistance(mesh_, snapParams, pp)
    };
    // Construct iterative mesh mover.
    Info << "Constructing mesh displacer ..." << endl;
    Info << "Using mesh parameters " << motionDict << nl << endl;
    const pointMesh& pMesh = pointMesh::New(mesh_);
    motionSmoother meshMover
    {
      mesh_,
      pp,
      adaptPatchIDs,
      meshRefinement::makeDisplacementField(pMesh, adaptPatchIDs)(),
      motionDict
    };
    // Check initial mesh
    Info << "Checking initial mesh ..." << endl;
    labelHashSet wrongFaces{mesh_.nFaces()/100};
    motionSmoother::checkMesh(false, mesh_, motionDict, wrongFaces);
    const label nInitErrors = returnReduce(wrongFaces.size(), sumOp<label>());
    Info << "Detected " << nInitErrors << " illegal faces"
      << " (concave, zero area or negative cell pyramid volume)"
      << endl;
    Info << "Checked initial mesh in = "
      << mesh_.time().cpuTimeIncrement() << " s\n" << nl << endl;
    // Pre-smooth patch vertices (so before determining nearest)
    autoSnapDriver::preSmoothPatch
      (
        *this,
        snapParams,
        nInitErrors,
        List<labelPair>(0), //baffles
        meshMover
      );
    pointField nearestPoint;
    vectorField nearestNormal;
    const vectorField disp
    {
      autoSnapDriver::calcNearestSurface
      (
        *this,
        snapDist,   // attraction
        pp,
        nearestPoint,
        nearestNormal
      )
    };
    const labelList& meshPoints = pp.meshPoints();
    pointField newPoints{mesh_.points()};
    FOR_ALL(meshPoints, i) {
      newPoints[meshPoints[i]] += disp[i];
    }
    syncTools::syncPointList
      (
        mesh_,
        newPoints,
        minMagSqrEqOp<point>(),     // combine op
        vector(GREAT, GREAT, GREAT) // null value (note: cannot use VGREAT)
      );
    mesh_.movePoints(newPoints);
  }
  // Per face the nearest adaptPatch
  const labelList nearestAdaptPatch{nearestPatch(meshedPatches())};
  // Per face (internal or coupled!) the patch that the
  // baffle should get (or -1).
  labelList facePatch{mesh_.nFaces(), -1};
  // Count of baffled faces
  label nBaffleFaces = 0;

  {
    faceSet wrongFaces{mesh_, "wrongFaces", 100};

    {
      //motionSmoother::checkMesh(false, mesh_, motionDict, wrongFaces);
      // Just check the errors from squashing
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      const labelList allFaces{identity(mesh_.nFaces())};
      label nWrongFaces = 0;
      scalar minArea{readScalar(motionDict.lookup("minArea"))};
      if (minArea > -SMALL) {
        polyMeshGeometry::checkFaceArea
          (
            false,
            minArea,
            mesh_,
            mesh_.faceAreas(),
            allFaces,
            &wrongFaces
          );
        label nNewWrongFaces = returnReduce(wrongFaces.size(), sumOp<label>());
        Info << "    faces with area < "
          << setw(5) << minArea
          << " m^2                            : "
          << nNewWrongFaces-nWrongFaces << endl;
        nWrongFaces = nNewWrongFaces;
      }
      scalar minDet{readScalar(motionDict.lookup("minDeterminant"))};
      if (minDet > -1) {
        polyMeshGeometry::checkCellDeterminant
          (
            false,
            minDet,
            mesh_,
            mesh_.faceAreas(),
            allFaces,
            polyMeshGeometry::affectedCells(mesh_, allFaces),
            &wrongFaces
          );
        label nNewWrongFaces = returnReduce(wrongFaces.size(), sumOp<label>());
        Info << "    faces on cells with determinant < "
          << setw(5) << minDet << "                : "
          << nNewWrongFaces-nWrongFaces << endl;
        nWrongFaces = nNewWrongFaces;
      }
    }
    FOR_ALL_CONST_ITER(faceSet, wrongFaces, iter) {
      label patchI = mesh_.boundaryMesh().whichPatch(iter.key());
      if (patchI == -1 || mesh_.boundaryMesh()[patchI].coupled()) {
        facePatch[iter.key()] = nearestAdaptPatch[iter.key()];
        nBaffleFaces++;
      }
    }
  }
  // Restore points.
  mesh_.movePoints(oldPoints);
  Info << "markFacesOnProblemCellsGeometric : marked "
    << returnReduce(nBaffleFaces, sumOp<label>())
    << " additional internal and coupled faces"
    << " to be converted into baffles." << endl;
  syncTools::syncFaceList
    (
      mesh_,
      facePatch,
      maxEqOp<label>()
    );
  return facePatch;
}

