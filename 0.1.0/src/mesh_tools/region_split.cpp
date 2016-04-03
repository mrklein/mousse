// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_split.hpp"
#include "cyclic_poly_patch.hpp"
#include "processor_poly_patch.hpp"
#include "global_index.hpp"
#include "sync_tools.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(regionSplit, 0);

}


// Private Member Functions

// Handle (non-processor) coupled faces.
void mousse::regionSplit::transferCoupledFaceRegion
(
  const label faceI,
  const label otherFaceI,
  labelList& faceRegion,
  DynamicList<label>& newChangedFaces
) const
{
  if (faceRegion[faceI] >= 0) {
    if (faceRegion[otherFaceI] == -1) {
      faceRegion[otherFaceI] = faceRegion[faceI];
      newChangedFaces.append(otherFaceI);
    } else if (faceRegion[otherFaceI] == -2) {
      // otherFaceI blocked but faceI is not. Is illegal for coupled
      // faces, not for explicit connections.
    } else if (faceRegion[otherFaceI] != faceRegion[faceI]) {
      FATAL_ERROR_IN
      (
         "regionSplit::transferCoupledFaceRegion"
         "(const label, const label, labelList&, labelList&) const"
      )
      << "Problem : coupled face " << faceI
      << " on patch " << mesh().boundaryMesh().whichPatch(faceI)
      << " has region " << faceRegion[faceI]
      << " but coupled face " << otherFaceI
      << " has region " << faceRegion[otherFaceI]
      << endl
      << "Is your blocked faces specification"
      << " synchronized across coupled boundaries?"
      << abort(FatalError);
    }
  } else if (faceRegion[faceI] == -1) {
    if (faceRegion[otherFaceI] >= 0) {
      faceRegion[faceI] = faceRegion[otherFaceI];
      newChangedFaces.append(faceI);
    } else if (faceRegion[otherFaceI] == -2) {
      // otherFaceI blocked but faceI is not. Is illegal for coupled
      // faces, not for explicit connections.
    }
  }
}


void mousse::regionSplit::fillSeedMask
(
  const List<labelPair>& explicitConnections,
  labelList& cellRegion,
  labelList& faceRegion,
  const label seedCellID,
  const label markValue
) const
{
  // Do seed cell
  cellRegion[seedCellID] = markValue;
  // Collect faces on seed cell
  const cell& cFaces = mesh().cells()[seedCellID];
  label nFaces = 0;
  labelList changedFaces{cFaces.size()};
  FOR_ALL(cFaces, i) {
    label faceI = cFaces[i];
    if (faceRegion[faceI] == -1) {
      faceRegion[faceI] = markValue;
      changedFaces[nFaces++] = faceI;
    }
  }
  changedFaces.setSize(nFaces);
  // Loop over changed faces. FaceCellWave in small.
  while (changedFaces.size()) {
    DynamicList<label> changedCells{changedFaces.size()};
    FOR_ALL(changedFaces, i) {
      label faceI = changedFaces[i];
      label own = mesh().faceOwner()[faceI];
      if (cellRegion[own] == -1) {
        cellRegion[own] = markValue;
        changedCells.append(own);
      }
      if (mesh().isInternalFace(faceI)) {
        label nei = mesh().faceNeighbour()[faceI];
        if (cellRegion[nei] == -1) {
          cellRegion[nei] = markValue;
          changedCells.append(nei);
        }
      }
    }
    // Loop over changedCells and collect faces
    DynamicList<label> newChangedFaces{changedCells.size()};
    FOR_ALL(changedCells, i) {
      label cellI = changedCells[i];
      const cell& cFaces = mesh().cells()[cellI];
      FOR_ALL(cFaces, cFaceI) {
        label faceI = cFaces[cFaceI];
        if (faceRegion[faceI] == -1) {
          faceRegion[faceI] = markValue;
          newChangedFaces.append(faceI);
        }
      }
    }
    // Check for changes to any locally coupled face.
    // Global connections are done later.
    const polyBoundaryMesh& patches = mesh().boundaryMesh();
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (isA<cyclicPolyPatch>(pp)
          && refCast<const cyclicPolyPatch>(pp).owner()) {
        // Transfer from neighbourPatch to here or vice versa.
        const cyclicPolyPatch& cycPatch = refCast<const cyclicPolyPatch>(pp);
        label faceI = cycPatch.start();
        FOR_ALL(cycPatch, i) {
          label otherFaceI = cycPatch.transformGlobalFace(faceI);
          transferCoupledFaceRegion
          (
            faceI,
            otherFaceI,
            faceRegion,
            newChangedFaces
          );
          faceI++;
        }
      }
    }
    FOR_ALL(explicitConnections, i) {
      transferCoupledFaceRegion
      (
        explicitConnections[i][0],
        explicitConnections[i][1],
        faceRegion,
        newChangedFaces
      );
    }
    changedFaces.transfer(newChangedFaces);
  }
}


mousse::label mousse::regionSplit::calcLocalRegionSplit
(
  const boolList& blockedFace,
  const List<labelPair>& explicitConnections,
  labelList& cellRegion
) const
{
  if (debug) {
    if (blockedFace.size()) {
      // Check that blockedFace is synced.
      boolList syncBlockedFace(blockedFace);
      syncTools::swapFaceList(mesh(), syncBlockedFace);
      FOR_ALL(syncBlockedFace, faceI) {
        if (syncBlockedFace[faceI] == blockedFace[faceI])
          continue;
        FATAL_ERROR_IN
        (
          "regionSplit::calcLocalRegionSplit(..)"
        )
        << "Face " << faceI << " not synchronised. My value:"
        << blockedFace[faceI] << "  coupled value:"
        << syncBlockedFace[faceI]
        << abort(FatalError);
      }
    }
  }
  // Region per face.
  // -1 unassigned
  // -2 blocked
  labelList faceRegion(mesh().nFaces(), -1);
  if (blockedFace.size()) {
    FOR_ALL(blockedFace, faceI) {
      if (blockedFace[faceI]) {
        faceRegion[faceI] = -2;
      }
    }
  }
  // Assign local regions
  // Start with region 0
  label nLocalRegions = 0;
  label unsetCellI = 0;
  do {
    // Find first unset cell
    for (; unsetCellI < mesh().nCells(); unsetCellI++) {
      if (cellRegion[unsetCellI] == -1) {
        break;
      }
    }
    if (unsetCellI >= mesh().nCells()) {
      break;
    }
    fillSeedMask
    (
      explicitConnections,
      cellRegion,
      faceRegion,
      unsetCellI,
      nLocalRegions
    );
    // Current unsetCell has now been handled. Go to next region.
    nLocalRegions++;
    unsetCellI++;
  } while (true);
  if (debug) {
    FOR_ALL(cellRegion, cellI) {
      if (cellRegion[cellI] < 0) {
        FATAL_ERROR_IN("regionSplit::calcLocalRegionSplit(..)")
          << "cell:" << cellI << " region:" << cellRegion[cellI]
          << abort(FatalError);
      }
    }
    FOR_ALL(faceRegion, faceI) {
      if (faceRegion[faceI] == -1) {
        FATAL_ERROR_IN("regionSplit::calcLocalRegionSplit(..)")
          << "face:" << faceI << " region:" << faceRegion[faceI]
          << abort(FatalError);
      }
    }
  }
  return nLocalRegions;
}


mousse::autoPtr<mousse::globalIndex> mousse::regionSplit::calcRegionSplit
(
  const bool doGlobalRegions,
  const boolList& blockedFace,
  const List<labelPair>& explicitConnections,
  labelList& cellRegion
) const
{
  // See header in regionSplit.H
  // 1. Do local analysis
  label nLocalRegions = calcLocalRegionSplit
  (
    blockedFace,
    explicitConnections,
    cellRegion
  );
  if (!doGlobalRegions) {
    return autoPtr<globalIndex>{new globalIndex{nLocalRegions}};
  }
  // 2. Assign global regions
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  // Offset local regions to create unique global regions.
  globalIndex globalRegions{nLocalRegions};
  // Convert regions to global ones
  FOR_ALL(cellRegion, cellI) {
    cellRegion[cellI] = globalRegions.toGlobal(cellRegion[cellI]);
  }
  // 3. Merge global regions
  // ~~~~~~~~~~~~~~~~~~~~~~~
  // Regions across non-blocked proc patches get merged.
  // This will set merged global regions to be the min of both.
  // (this will create gaps in the global region list so they will get
  // merged later on)
  while (true) {
    if (debug) {
      Pout << nl << "-- Starting Iteration --" << endl;
    }
    const polyBoundaryMesh& patches = mesh().boundaryMesh();
    labelList nbrRegion(mesh().nFaces()-mesh().nInternalFaces(), -1);
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (pp.coupled()) {
        const labelList& patchCells = pp.faceCells();
        SubList<label> patchNbrRegion
        {
          nbrRegion,
          pp.size(),
          pp.start()-mesh().nInternalFaces()
        };
        FOR_ALL(patchCells, i) {
          label faceI = pp.start()+i;
          if (!blockedFace.size() || !blockedFace[faceI]) {
            patchNbrRegion[i] = cellRegion[patchCells[i]];
          }
        }
      }
    }
    syncTools::swapBoundaryFaceList(mesh(), nbrRegion);
    Map<label> globalToMerged{mesh().nFaces()-mesh().nInternalFaces()};
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (!pp.coupled())
        continue;
      const labelList& patchCells = pp.faceCells();
      SubList<label> patchNbrRegion
      {
        nbrRegion,
        pp.size(),
        pp.start()-mesh().nInternalFaces()
      };
      FOR_ALL(patchCells, i) {
        label faceI = pp.start()+i;
        if (blockedFace.size() && blockedFace[faceI])
          continue;
        if (patchNbrRegion[i] < cellRegion[patchCells[i]]) {
          globalToMerged.insert
          (
            cellRegion[patchCells[i]],
            patchNbrRegion[i]
          );
        }
      }
    }
    label nMerged = returnReduce(globalToMerged.size(), sumOp<label>());
    if (debug) {
      Pout << "nMerged:" << nMerged << endl;
    }
    if (nMerged == 0) {
      break;
    }
    // Renumber the regions according to the globalToMerged
    FOR_ALL(cellRegion, cellI) {
      label regionI = cellRegion[cellI];
      Map<label>::const_iterator iter = globalToMerged.find(regionI);
      if (iter != globalToMerged.end()) {
        cellRegion[cellI] = iter();
      }
    }
  }
  // Now our cellRegion will have non-local elements in it. So compact
  // it.
  // 4a: count. Use a labelHashSet to count regions only once.
  label nCompact = 0;

  {
    labelHashSet localRegion{mesh().nFaces()-mesh().nInternalFaces()};
    FOR_ALL(cellRegion, cellI) {
      if (globalRegions.isLocal(cellRegion[cellI])
          && localRegion.insert(cellRegion[cellI])) {
        nCompact++;
      }
    }
  }
  if (debug) {
    Pout << "Compacted from " << nLocalRegions
      << " down to " << nCompact << " local regions." << endl;
  }
  // Global numbering for compacted local regions
  autoPtr<globalIndex> globalCompactPtr{new globalIndex{nCompact}};
  const globalIndex& globalCompact = globalCompactPtr();
  // 4b: renumber
  // Renumber into compact indices. Note that since we've already made
  // all regions global we now need a Map to store the compacting information
  // instead of a labelList - otherwise we could have used a straight
  // labelList.
  // Local compaction map
  Map<label> globalToCompact{2*nCompact};
  // Remote regions we want the compact number for
  List<labelHashSet> nonLocal{Pstream::nProcs()};
  FOR_ALL(nonLocal, procI) {
    nonLocal[procI].resize((nLocalRegions-nCompact)/Pstream::nProcs());
  }
  FOR_ALL(cellRegion, cellI) {
    label region = cellRegion[cellI];
    if (globalRegions.isLocal(region)) {
      Map<label>::const_iterator iter = globalToCompact.find(region);
      if (iter == globalToCompact.end()) {
        label compactRegion = globalCompact.toGlobal
        (
          globalToCompact.size()
        );
        globalToCompact.insert(region, compactRegion);
      }
    } else {
      nonLocal[globalRegions.whichProcID(region)].insert(region);
    }
  }
  // Now we have all the local regions compacted. Now we need to get the
  // non-local ones from the processors to whom they are local.
  // Convert the nonLocal (labelHashSets) to labelLists.
  labelListList sendNonLocal{Pstream::nProcs()};
  labelList nNonLocal{Pstream::nProcs(), 0};
  FOR_ALL(sendNonLocal, procI) {
    sendNonLocal[procI].setSize(nonLocal[procI].size());
    FOR_ALL_CONST_ITER(labelHashSet, nonLocal[procI], iter) {
      sendNonLocal[procI][nNonLocal[procI]++] = iter.key();
    }
  }
  if (debug) {
    FOR_ALL(nNonLocal, procI) {
      Pout << "    from processor " << procI
        << " want " << nNonLocal[procI]
        << " region numbers."
        << endl;
    }
    Pout<< endl;
  }
  // Get the wanted region labels into recvNonLocal
  labelListList recvNonLocal;
  labelListList sizes;
  Pstream::exchange<labelList, label>
  (
    sendNonLocal,
    recvNonLocal,
    sizes
  );
  // Now we have the wanted compact region labels that procI wants in
  // recvNonLocal[procI]. Construct corresponding list of compact
  // region labels to send back.
  labelListList sendWantedLocal{Pstream::nProcs()};
  FOR_ALL(recvNonLocal, procI) {
    const labelList& nonLocal = recvNonLocal[procI];
    sendWantedLocal[procI].setSize(nonLocal.size());
    FOR_ALL(nonLocal, i) {
      sendWantedLocal[procI][i] = globalToCompact[nonLocal[i]];
    }
  }
  // Send back (into recvNonLocal)
  recvNonLocal.clear();
  sizes.clear();
  Pstream::exchange<labelList, label>
  (
    sendWantedLocal,
    recvNonLocal,
    sizes
  );
  sendWantedLocal.clear();
  // Now recvNonLocal contains for every element in setNonLocal the
  // corresponding compact number. Insert these into the local compaction
  // map.
  FOR_ALL(recvNonLocal, procI) {
    const labelList& wantedRegions = sendNonLocal[procI];
    const labelList& compactRegions = recvNonLocal[procI];
    FOR_ALL(wantedRegions, i) {
      globalToCompact.insert(wantedRegions[i], compactRegions[i]);
    }
  }
  // Finally renumber the regions
  FOR_ALL(cellRegion, cellI) {
    cellRegion[cellI] = globalToCompact[cellRegion[cellI]];
  }
  return globalCompactPtr;
}


// Constructors
mousse::regionSplit::regionSplit(const polyMesh& mesh, const bool doGlobalRegions)
:
  MeshObject<polyMesh, mousse::TopologicalMeshObject, regionSplit>{mesh},
  labelList{mesh.nCells(), -1}
{
  globalNumberingPtr_ = calcRegionSplit
  (
    doGlobalRegions,    //do global regions
    boolList{0, false}, //blockedFaces
    List<labelPair>{0}, //explicitConnections,
    *this
  );
}


mousse::regionSplit::regionSplit
(
  const polyMesh& mesh,
  const boolList& blockedFace,
  const bool doGlobalRegions
)
:
  MeshObject<polyMesh, mousse::TopologicalMeshObject, regionSplit>{mesh},
  labelList{mesh.nCells(), -1}
{
  globalNumberingPtr_ = calcRegionSplit
  (
    doGlobalRegions,
    blockedFace,        //blockedFaces
    List<labelPair>{0}, //explicitConnections,
    *this
  );
}


mousse::regionSplit::regionSplit
(
  const polyMesh& mesh,
  const boolList& blockedFace,
  const List<labelPair>& explicitConnections,
  const bool doGlobalRegions
)
:
  MeshObject<polyMesh, mousse::TopologicalMeshObject, regionSplit>{mesh},
  labelList{mesh.nCells(), -1}
{
  globalNumberingPtr_ = calcRegionSplit
  (
    doGlobalRegions,
    blockedFace,            //blockedFaces
    explicitConnections,    //explicitConnections,
    *this
  );
}

