// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "ioobject_list.hpp"
#include "fv_mesh.hpp"
#include "poly_topo_change.hpp"
#include "_read_fields.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "sortable_list.hpp"
#include "decomposition_method.hpp"
#include "renumber_method.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "cuthill_mc_kee_renumber.hpp"
#include "fv_mesh_subset.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"


using namespace mousse;


// Create named field from labelList for postprocessing
tmp<volScalarField> createScalarField
(
  const fvMesh& mesh,
  const word& name,
  const labelList& elems
)
{
  tmp<volScalarField> tfld
  {
    new volScalarField
    {
      {
        name,
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE,
        false
      },
      mesh,
      {"zero", dimless, 0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  volScalarField& fld = tfld();
  FOR_ALL(fld, cellI) {
   fld[cellI] = elems[cellI];
  }
  return tfld;
}


// Calculate band of matrix
label getBand(const labelList& owner, const labelList& neighbour)
{
  label band = 0;
  FOR_ALL(neighbour, faceI) {
    label diff = neighbour[faceI] - owner[faceI];
    if (diff > band) {
      band = diff;
    }
  }
  return band;
}


// Calculate band of matrix
void getBand
(
  const bool calculateIntersect,
  const label nCells,
  const labelList& owner,
  const labelList& neighbour,
  label& bandwidth,
  scalar& profile,            // scalar to avoid overflow
  scalar& sumSqrIntersect     // scalar to avoid overflow
)
{
  labelList cellBandwidth{nCells, 0};
  scalarField nIntersect{nCells, 0.0};
  FOR_ALL(neighbour, faceI) {
    label own = owner[faceI];
    label nei = neighbour[faceI];
    // Note: mag not necessary for correct (upper-triangular) ordering.
    label diff = nei-own;
    cellBandwidth[nei] = max(cellBandwidth[nei], diff);
  }
  bandwidth = max(cellBandwidth);
  // Do not use field algebra because of conversion label to scalar
  profile = 0.0;
  FOR_ALL(cellBandwidth, cellI) {
    profile += 1.0*cellBandwidth[cellI];
  }
  sumSqrIntersect = 0.0;
  if (!calculateIntersect)
    return;
  FOR_ALL(nIntersect, cellI) {
    for (label colI = cellI-cellBandwidth[cellI]; colI <= cellI; colI++) {
      nIntersect[colI] += 1.0;
    }
  }
  sumSqrIntersect = sum(mousse::sqr(nIntersect));
}


// Determine upper-triangular face order
labelList getFaceOrder
(
  const primitiveMesh& mesh,
  const labelList& cellOrder      // New to old cell
)
{
  labelList reverseCellOrder{invert(cellOrder.size(), cellOrder)};
  labelList oldToNewFace{mesh.nFaces(), -1};
  label newFaceI = 0;
  labelList nbr;
  labelList order;
  FOR_ALL(cellOrder, newCellI) {
    label oldCellI = cellOrder[newCellI];
    const cell& cFaces = mesh.cells()[oldCellI];
    // Neighbouring cells
    nbr.setSize(cFaces.size());
    FOR_ALL(cFaces, i) {
      label faceI = cFaces[i];
      if (mesh.isInternalFace(faceI)) {
        // Internal face. Get cell on other side.
        label nbrCellI = reverseCellOrder[mesh.faceNeighbour()[faceI]];
        if (nbrCellI == newCellI) {
          nbrCellI = reverseCellOrder[mesh.faceOwner()[faceI]];
        }
        nbr[i] = (newCellI < nbrCellI) ? nbrCellI : -1;
      } else {
        // External face. Do later.
        nbr[i] = -1;
      }
    }
    order.setSize(nbr.size());
    sortedOrder(nbr, order);
    FOR_ALL(order, i) {
      label index = order[i];
      if (nbr[index] != -1) {
        oldToNewFace[cFaces[index]] = newFaceI++;
      }
    }
  }
  // Leave patch faces intact.
  for (label faceI = newFaceI; faceI < mesh.nFaces(); faceI++) {
    oldToNewFace[faceI] = faceI;
  }
  // Check done all faces.
  FOR_ALL(oldToNewFace, faceI) {
    if (oldToNewFace[faceI] == -1) {
      FATAL_ERROR_IN
      (
        "getFaceOrder"
        "(const primitiveMesh&, const labelList&, const labelList&)"
      )
      << "Did not determine new position" << " for face " << faceI
      << abort(FatalError);
    }
  }
  return invert(mesh.nFaces(), oldToNewFace);
}


// Determine face order such that inside region faces are sorted
// upper-triangular but inbetween region faces are handled like boundary faces.
labelList getRegionFaceOrder
(
  const primitiveMesh& mesh,
  const labelList& cellOrder,     // New to old cell
  const labelList& cellToRegion   // Old cell to region
)
{
  labelList reverseCellOrder{invert(cellOrder.size(), cellOrder)};
  labelList oldToNewFace{mesh.nFaces(), -1};
  label newFaceI = 0;
  label prevRegion = -1;
  FOR_ALL(cellOrder, newCellI) {
    label oldCellI = cellOrder[newCellI];
    if (cellToRegion[oldCellI] != prevRegion) {
      prevRegion = cellToRegion[oldCellI];
    }
    const cell& cFaces = mesh.cells()[oldCellI];
    SortableList<label> nbr{cFaces.size()};
    FOR_ALL(cFaces, i) {
      label faceI = cFaces[i];
      if (mesh.isInternalFace(faceI)) {
        // Internal face. Get cell on other side.
        label nbrCellI = reverseCellOrder[mesh.faceNeighbour()[faceI]];
        if (nbrCellI == newCellI) {
          nbrCellI = reverseCellOrder[mesh.faceOwner()[faceI]];
        }
        if (cellToRegion[oldCellI] != cellToRegion[cellOrder[nbrCellI]]) {
          // Treat like external face. Do later.
          nbr[i] = -1;
        } else if (newCellI < nbrCellI) {
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
        oldToNewFace[cFaces[nbr.indices()[i]]] = newFaceI++;
      }
    }
  }

  // Do region interfaces
  label nRegions = max(cellToRegion)+1;

  {
    // Sort in increasing region
    SortableList<label> sortKey{mesh.nFaces(), labelMax};
    for (label faceI = 0; faceI < mesh.nInternalFaces(); faceI++) {
      label ownRegion = cellToRegion[mesh.faceOwner()[faceI]];
      label neiRegion = cellToRegion[mesh.faceNeighbour()[faceI]];
      if (ownRegion != neiRegion) {
        sortKey[faceI] =
          min(ownRegion, neiRegion)*nRegions + max(ownRegion, neiRegion);
      }
    }
    sortKey.sort();
    // Extract.
    label prevKey = -1;
    FOR_ALL(sortKey, i) {
      label key = sortKey[i];
      if (key == labelMax) {
        break;
      }
      if (prevKey != key) {
        prevKey = key;
      }
      oldToNewFace[sortKey.indices()[i]] = newFaceI++;
    }
  }
  // Leave patch faces intact.
  for (label faceI = newFaceI; faceI < mesh.nFaces(); faceI++) {
    oldToNewFace[faceI] = faceI;
  }
  // Check done all faces.
  FOR_ALL(oldToNewFace, faceI) {
    if (oldToNewFace[faceI] == -1) {
      FATAL_ERROR_IN
      (
        "getRegionFaceOrder"
        "(const primitveMesh&, const labelList&, const labelList&)"
      )
      << "Did not determine new position"
      << " for face " << faceI
      << abort(FatalError);
    }
  }
  return invert(mesh.nFaces(), oldToNewFace);
}


// cellOrder: old cell for every new cell
// faceOrder: old face for every new face. Ordering of boundary faces not
//     changed.
autoPtr<mapPolyMesh> reorderMesh
(
  polyMesh& mesh,
  const labelList& cellOrder,
  const labelList& faceOrder
)
{
  labelList reverseCellOrder{invert(cellOrder.size(), cellOrder)};
  labelList reverseFaceOrder{invert(faceOrder.size(), faceOrder)};
  faceList newFaces{reorder(reverseFaceOrder, mesh.faces())};
  labelList newOwner
  {
    renumber(reverseCellOrder, reorder(reverseFaceOrder, mesh.faceOwner()))
  };
  labelList newNeighbour
  {
    renumber(reverseCellOrder, reorder(reverseFaceOrder, mesh.faceNeighbour()))
  };
  // Check if any faces need swapping.
  labelHashSet flipFaceFlux{newOwner.size()};
  FOR_ALL(newNeighbour, faceI) {
    label own = newOwner[faceI];
    label nei = newNeighbour[faceI];
    if (nei < own) {
      newFaces[faceI].flip();
      Swap(newOwner[faceI], newNeighbour[faceI]);
      flipFaceFlux.insert(faceI);
    }
  }
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  labelList patchSizes{patches.size()};
  labelList patchStarts{patches.size()};
  labelList oldPatchNMeshPoints{patches.size()};
  labelListList patchPointMap{patches.size()};
  FOR_ALL(patches, patchI) {
    patchSizes[patchI] = patches[patchI].size();
    patchStarts[patchI] = patches[patchI].start();
    oldPatchNMeshPoints[patchI] = patches[patchI].nPoints();
    patchPointMap[patchI] = identity(patches[patchI].nPoints());
  }
  mesh.resetPrimitives
  (
    Xfer<pointField>::null(),
    xferMove(newFaces),
    xferMove(newOwner),
    xferMove(newNeighbour),
    patchSizes,
    patchStarts,
    true
  );
  // Re-do the faceZones
  {
    faceZoneMesh& faceZones = mesh.faceZones();
    faceZones.clearAddressing();
    FOR_ALL(faceZones, zoneI) {
      faceZone& fZone = faceZones[zoneI];
      labelList newAddressing{fZone.size()};
      boolList newFlipMap{fZone.size()};
      FOR_ALL(fZone, i) {
        label oldFaceI = fZone[i];
        newAddressing[i] = reverseFaceOrder[oldFaceI];
        if (flipFaceFlux.found(newAddressing[i])) {
          newFlipMap[i] = !fZone.flipMap()[i];
        } else {
          newFlipMap[i] = fZone.flipMap()[i];
        }
      }
      labelList newToOld;
      sortedOrder(newAddressing, newToOld);
      fZone.resetAddressing
      (
        UIndirectList<label>{newAddressing, newToOld}(),
        UIndirectList<bool>{newFlipMap, newToOld}()
      );
    }
  }
  // Re-do the cellZones
  {
    cellZoneMesh& cellZones = mesh.cellZones();
    cellZones.clearAddressing();
    FOR_ALL(cellZones, zoneI) {
      cellZones[zoneI] =
        UIndirectList<label>{reverseCellOrder, cellZones[zoneI]}();
      mousse::sort(cellZones[zoneI]);
    }
  }
  return
    autoPtr<mapPolyMesh>
    {
      new mapPolyMesh
      {
        mesh,                       // const polyMesh& mesh,
        mesh.nPoints(),             // nOldPoints,
        mesh.nFaces(),              // nOldFaces,
        mesh.nCells(),              // nOldCells,
        identity(mesh.nPoints()),   // pointMap,
        List<objectMap>{0},         // pointsFromPoints,
        faceOrder,                  // faceMap,
        List<objectMap>{0},         // facesFromPoints,
        List<objectMap>{0},         // facesFromEdges,
        List<objectMap>{0},         // facesFromFaces,
        cellOrder,                  // cellMap,
        List<objectMap>{0},         // cellsFromPoints,
        List<objectMap>{0},         // cellsFromEdges,
        List<objectMap>{0},         // cellsFromFaces,
        List<objectMap>{0},         // cellsFromCells,
        identity(mesh.nPoints()),   // reversePointMap,
        reverseFaceOrder,           // reverseFaceMap,
        reverseCellOrder,           // reverseCellMap,
        flipFaceFlux,               // flipFaceFlux,
        patchPointMap,              // patchPointMap,
        labelListList{0},           // pointZoneMap,
        labelListList{0},           // faceZonePointMap,
        labelListList{0},           // faceZoneFaceMap,
        labelListList{0},           // cellZoneMap,
        pointField{0},              // preMotionPoints,
        patchStarts,                // oldPatchStarts,
        oldPatchNMeshPoints,        // oldPatchNMeshPoints
        autoPtr<scalarField>{}      // oldCellVolumes
      }
    };
}


// Return new to old cell numbering
labelList regionRenumber
(
  const renumberMethod& method,
  const fvMesh& mesh,
  const labelList& cellToRegion
)
{
  Info << "Determining cell order:" << endl;
  labelList cellOrder{cellToRegion.size()};
  label nRegions = max(cellToRegion)+1;
  labelListList regionToCells{invertOneToMany(nRegions, cellToRegion)};
  label cellI = 0;
  FOR_ALL(regionToCells, regionI) {
    Info << "    region " << regionI << " starts at " << cellI << endl;
    // Make sure no parallel comms
    bool oldParRun = UPstream::parRun();
    UPstream::parRun() = false;
    // Per region do a reordering.
    fvMeshSubset subsetter{mesh};
    subsetter.setLargeCellSubset(cellToRegion, regionI);
    const fvMesh& subMesh = subsetter.subMesh();
    labelList subCellOrder =
      method.renumber(subMesh, subMesh.cellCentres());
    // Restore state
    UPstream::parRun() = oldParRun;
    const labelList& cellMap = subsetter.cellMap();
    FOR_ALL(subCellOrder, i) {
      cellOrder[cellI++] = cellMap[subCellOrder[i]];
    }
  }
  Info << endl;
  return cellOrder;
}


int main(int argc, char *argv[])
{
  argList::addNote("Renumber mesh to minimise bandwidth");
  #include "add_region_option.inc"
  #include "add_overwrite_option.inc"
  #include "add_time_options.inc"
  #include "add_dict_option.inc"
  argList::addBoolOption
  (
    "frontWidth",
    "calculate the rms of the frontwidth"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  // Get times list
  instantList Times = runTime.times();
  // Set startTime and endTime depending on -time and -latestTime options
  #include "check_time_options.inc"
  runTime.setTime(Times[startTime], startTime);
  #include "create_named_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const bool readDict = args.optionFound("dict");
  const bool doFrontWidth = args.optionFound("frontWidth");
  const bool overwrite = args.optionFound("overwrite");
  label band;
  scalar profile;
  scalar sumSqrIntersect;
  getBand
  (
    doFrontWidth,
    mesh.nCells(),
    mesh.faceOwner(),
    mesh.faceNeighbour(),
    band,
    profile,
    sumSqrIntersect
  );
  reduce(band, maxOp<label>());
  reduce(profile, sumOp<scalar>());
  scalar rmsFrontwidth =
    mousse::sqrt
    (
      returnReduce(sumSqrIntersect, sumOp<scalar>())
      /mesh.globalData().nTotalCells()
    );
  Info << "Mesh size: " << mesh.globalData().nTotalCells() << nl
    << "Before renumbering :" << nl
    << "    band           : " << band << nl
    << "    profile        : " << profile << nl;
  if (doFrontWidth) {
    Info << "    rms frontwidth : " << rmsFrontwidth << nl;
  }
  Info << endl;
  bool sortCoupledFaceCells = false;
  bool writeMaps = false;
  bool orderPoints = false;
  label blockSize = 0;
  bool renumberSets = true;
  // Construct renumberMethod
  autoPtr<IOdictionary> renumberDictPtr;
  autoPtr<renumberMethod> renumberPtr;
  if (readDict) {
    const word dictName{"renumberMeshDict"};
    #include "set_system_mesh_dictionary_io.inc"
    Info << "Renumber according to " << dictName << nl << endl;
    renumberDictPtr.reset(new IOdictionary{dictIO});
    const IOdictionary& renumberDict = renumberDictPtr();
    renumberPtr = renumberMethod::New(renumberDict);
    sortCoupledFaceCells =
      renumberDict.lookupOrDefault("sortCoupledFaceCells", false);
    if (sortCoupledFaceCells) {
      Info << "Sorting cells on coupled boundaries to be last." << nl
        << endl;
    }
    blockSize = renumberDict.lookupOrDefault("blockSize", 0);
    if (blockSize > 0) {
      Info << "Ordering cells into regions of size " << blockSize
        << " (using decomposition);"
        << " ordering faces into region-internal and region-external."
        << nl << endl;
      if (blockSize < 0 || blockSize >= mesh.nCells()) {
        FATAL_ERROR_IN(args.executable())
          << "Block size " << blockSize
          << " should be positive integer"
          << " and less than the number of cells in the mesh."
          << exit(FatalError);
      }
    }
    orderPoints = renumberDict.lookupOrDefault("orderPoints", false);
    if (orderPoints) {
      Info << "Ordering points into internal and boundary points." << nl
        << endl;
    }
    renumberDict.lookup("writeMaps") >> writeMaps;
    if (writeMaps) {
      Info << "Writing renumber maps (new to old) to polyMesh." << nl
        << endl;
    }
    renumberSets = renumberDict.lookupOrDefault("renumberSets", true);
  } else {
    Info << "Using default renumberMethod." << nl << endl;
    dictionary renumberDict;
    renumberPtr.reset(new CuthillMcKeeRenumber{renumberDict});
  }
  Info << "Selecting renumberMethod " << renumberPtr().type() << nl << endl;
  // Read parallel reconstruct maps
  labelIOList cellProcAddressing
  {
    {
      "cellProcAddressing",
      mesh.facesInstance(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT
    },
    labelList{0}
  };
  labelIOList faceProcAddressing
  {
    {
      "faceProcAddressing",
      mesh.facesInstance(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT
    },
    labelList{0}
  };
  labelIOList pointProcAddressing
  {
    {
      "pointProcAddressing",
      mesh.pointsInstance(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT
    },
    labelList{0}
  };
  labelIOList boundaryProcAddressing
  {
    {
      "boundaryProcAddressing",
      mesh.pointsInstance(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT
    },
    labelList{0}
  };
  // Read objects in time directory
  IOobjectList objects{mesh, runTime.timeName()};
  // Read vol fields.
  PtrList<volScalarField> vsFlds;
  ReadFields(mesh, objects, vsFlds);
  PtrList<volVectorField> vvFlds;
  ReadFields(mesh, objects, vvFlds);
  PtrList<volSphericalTensorField> vstFlds;
  ReadFields(mesh, objects, vstFlds);
  PtrList<volSymmTensorField> vsymtFlds;
  ReadFields(mesh, objects, vsymtFlds);
  PtrList<volTensorField> vtFlds;
  ReadFields(mesh, objects, vtFlds);
  // Read surface fields.
  PtrList<surfaceScalarField> ssFlds;
  ReadFields(mesh, objects, ssFlds);
  PtrList<surfaceVectorField> svFlds;
  ReadFields(mesh, objects, svFlds);
  PtrList<surfaceSphericalTensorField> sstFlds;
  ReadFields(mesh, objects, sstFlds);
  PtrList<surfaceSymmTensorField> ssymtFlds;
  ReadFields(mesh, objects, ssymtFlds);
  PtrList<surfaceTensorField> stFlds;
  ReadFields(mesh, objects, stFlds);
  // Read point fields.
  PtrList<pointScalarField> psFlds;
  ReadFields(pointMesh::New(mesh), objects, psFlds);
  PtrList<pointVectorField> pvFlds;
  ReadFields(pointMesh::New(mesh), objects, pvFlds);
  PtrList<pointSphericalTensorField> pstFlds;
  ReadFields(pointMesh::New(mesh), objects, pstFlds);
  PtrList<pointSymmTensorField> psymtFlds;
  ReadFields(pointMesh::New(mesh), objects, psymtFlds);
  PtrList<pointTensorField> ptFlds;
  ReadFields(pointMesh::New(mesh), objects, ptFlds);
  Info << endl;
  // From renumbering:
  // - from new cell/face back to original cell/face
  labelList cellOrder;
  labelList faceOrder;
  if (blockSize > 0) {
    // Renumbering in two phases. Should be done in one so mapping of
    // fields is done correctly!
    label nBlocks = mesh.nCells()/blockSize;
    Info << "nBlocks   = " << nBlocks << endl;
    // Read decompositionMethod dictionary
    dictionary decomposeDict{renumberDictPtr().subDict("blockCoeffs")};
    decomposeDict.set("numberOfSubdomains", nBlocks);
    bool oldParRun = UPstream::parRun();
    UPstream::parRun() = false;
    auto decomposePtr = decompositionMethod::New(decomposeDict);
    labelList cellToRegion
    {
      decomposePtr().decompose(mesh, mesh.cellCentres())
    };
    // Restore state
    UPstream::parRun() = oldParRun;
    // For debugging: write out region
    createScalarField
    (
      mesh,
      "cellDist",
      cellToRegion
    )().write();
    Info << nl << "Written decomposition as volScalarField to "
      << "cellDist for use in postprocessing."
      << nl << endl;
    cellOrder = regionRenumber(renumberPtr(), mesh, cellToRegion);
    // Determine new to old face order with new cell numbering
    faceOrder = getRegionFaceOrder(mesh, cellOrder, cellToRegion);
  } else {
    // Detemines sorted back to original cell ordering
    cellOrder = renumberPtr().renumber(mesh, mesh.cellCentres());
    if (sortCoupledFaceCells) {
      // Change order so all coupled patch faceCells are at the end.
      const polyBoundaryMesh& pbm = mesh.boundaryMesh();
      // Collect all boundary cells on coupled patches
      label nBndCells = 0;
      FOR_ALL(pbm, patchI) {
        if (pbm[patchI].coupled()) {
          nBndCells += pbm[patchI].size();
        }
      }
      labelList reverseCellOrder = invert(mesh.nCells(), cellOrder);
      labelList bndCells{nBndCells};
      labelList bndCellMap{nBndCells};
      nBndCells = 0;
      FOR_ALL(pbm, patchI) {
        if (!pbm[patchI].coupled())
          continue;
        const labelUList& faceCells = pbm[patchI].faceCells();
        FOR_ALL(faceCells, i) {
          label cellI = faceCells[i];
          if (reverseCellOrder[cellI] != -1) {
            bndCells[nBndCells] = cellI;
            bndCellMap[nBndCells++] = reverseCellOrder[cellI];
            reverseCellOrder[cellI] = -1;
          }
        }
      }
      bndCells.setSize(nBndCells);
      bndCellMap.setSize(nBndCells);
      // Sort
      labelList order;
      sortedOrder(bndCellMap, order);
      // Redo newReverseCellOrder
      labelList newReverseCellOrder{mesh.nCells(), -1};
      label sortedI = mesh.nCells();
      FOR_ALL_REVERSE(order, i) {
        label origCellI = bndCells[order[i]];
        newReverseCellOrder[origCellI] = --sortedI;
      }
      Info << "Ordered all " << nBndCells << " cells with a coupled face"
        << " to the end of the cell list, starting at " << sortedI
        << endl;
      // Compact
      sortedI = 0;
      FOR_ALL(cellOrder, newCellI) {
        label origCellI = cellOrder[newCellI];
        if (newReverseCellOrder[origCellI] == -1) {
          newReverseCellOrder[origCellI] = sortedI++;
        }
      }
      // Update sorted back to original (unsorted) map
      cellOrder = invert(mesh.nCells(), newReverseCellOrder);
    }
    // Determine new to old face order with new cell numbering
    faceOrder = getFaceOrder(mesh, cellOrder);
  }
  if (!overwrite) {
    runTime++;
  }
  // Change the mesh.
  autoPtr<mapPolyMesh> map = reorderMesh(mesh, cellOrder, faceOrder);
  if (orderPoints) {
    polyTopoChange meshMod{mesh};
    autoPtr<mapPolyMesh> pointOrderMap =
      meshMod.changeMesh
      (
        mesh,
        false,      // inflate
        true,       // syncParallel
        false,      // orderCells
        orderPoints // orderPoints
      );
    // Combine point reordering into map.
    const_cast<labelList&>(map().pointMap()) =
      UIndirectList<label>(map().pointMap(), pointOrderMap().pointMap())();
    inplaceRenumber
    (
      pointOrderMap().reversePointMap(),
      const_cast<labelList&>(map().reversePointMap())
    );
  }
  // Update fields
  mesh.updateMesh(map);
  // Update proc maps
  if (cellProcAddressing.headerOk()
      && cellProcAddressing.size() == mesh.nCells()) {
    Info << "Renumbering processor cell decomposition map "
      << cellProcAddressing.name() << endl;
    cellProcAddressing =
      labelList{UIndirectList<label>{cellProcAddressing, map().cellMap()}};
  }
  if (faceProcAddressing.headerOk()
      && faceProcAddressing.size() == mesh.nFaces()) {
    Info << "Renumbering processor face decomposition map "
      << faceProcAddressing.name() << endl;
    faceProcAddressing =
      labelList{UIndirectList<label>{faceProcAddressing, map().faceMap()}};
    // Detect any flips.
    const labelHashSet& fff = map().flipFaceFlux();
    FOR_ALL_CONST_ITER(labelHashSet, fff, iter) {
      label faceI = iter.key();
      label masterFaceI = faceProcAddressing[faceI];
      faceProcAddressing[faceI] = -masterFaceI;
      if (masterFaceI == 0) {
        FATAL_ERROR_IN(args.executable()) << "problem faceI:" << faceI
          << " masterFaceI:" << masterFaceI << exit(FatalError);
      }
    }
  }
  if (pointProcAddressing.headerOk()
      && pointProcAddressing.size() == mesh.nPoints()) {
    Info << "Renumbering processor point decomposition map "
      << pointProcAddressing.name() << endl;
    pointProcAddressing =
      labelList{UIndirectList<label>{pointProcAddressing, map().pointMap()}};
  }
  // Move mesh (since morphing might not do this)
  if (map().hasMotionPoints()) {
    mesh.movePoints(map().preMotionPoints());
  }

  {
    label band;
    scalar profile;
    scalar sumSqrIntersect;
    getBand
    (
      doFrontWidth,
      mesh.nCells(),
      mesh.faceOwner(),
      mesh.faceNeighbour(),
      band,
      profile,
      sumSqrIntersect
    );
    reduce(band, maxOp<label>());
    reduce(profile, sumOp<scalar>());
    scalar rmsFrontwidth =
      mousse::sqrt
      (
        returnReduce
        (
          sumSqrIntersect,
          sumOp<scalar>()
        )/mesh.globalData().nTotalCells()
      );
    Info << "After renumbering :" << nl
      << "    band           : " << band << nl
      << "    profile        : " << profile << nl;
    if (doFrontWidth) {
      Info << "    rms frontwidth : " << rmsFrontwidth << nl;
    }
    Info << endl;
  }
  if (orderPoints) {
    // Force edge calculation (since only reason that points would need to
    // be sorted)
    (void)mesh.edges();
    label nTotPoints = returnReduce(mesh.nPoints(), sumOp<label>());
    label nTotIntPoints =
      returnReduce(mesh.nInternalPoints(), sumOp<label>());
    label nTotEdges =
      returnReduce(mesh.nEdges(), sumOp<label>());
    label nTotIntEdges =
      returnReduce(mesh.nInternalEdges(), sumOp<label>());
    label nTotInt0Edges =
      returnReduce(mesh.nInternal0Edges(), sumOp<label>());
    label nTotInt1Edges =
      returnReduce(mesh.nInternal1Edges(), sumOp<label>());
    Info << "Points:" << nl
      << "    total   : " << nTotPoints << nl
      << "    internal: " << nTotIntPoints << nl
      << "    boundary: " << nTotPoints-nTotIntPoints << nl
      << "Edges:" << nl
      << "    total   : " << nTotEdges << nl
      << "    internal: " << nTotIntEdges << nl
      << "        internal using 0 boundary points: "
      << nTotInt0Edges << nl
      << "        internal using 1 boundary points: "
      << nTotInt1Edges-nTotInt0Edges << nl
      << "        internal using 2 boundary points: "
      << nTotIntEdges-nTotInt1Edges << nl
      << "    boundary: " << nTotEdges-nTotIntEdges << nl
      << endl;
  }
  if (overwrite) {
    mesh.setInstance(oldInstance);
  }
  Info << "Writing mesh to " << mesh.facesInstance() << endl;
  mesh.write();
  if (cellProcAddressing.headerOk()) {
    cellProcAddressing.instance() = mesh.facesInstance();
    if (cellProcAddressing.size() == mesh.nCells()) {
      cellProcAddressing.write();
    } else {
      // procAddressing file no longer valid. Delete it.
      const fileName fName(cellProcAddressing.filePath());
      if (fName.size()) {
        Info << "Deleting inconsistent processor cell decomposition"
          << " map " << fName << endl;
        rm(fName);
      }
    }
  }
  if (faceProcAddressing.headerOk()) {
    faceProcAddressing.instance() = mesh.facesInstance();
    if (faceProcAddressing.size() == mesh.nFaces()) {
      faceProcAddressing.write();
    } else {
      const fileName fName(faceProcAddressing.filePath());
      if (fName.size()) {
        Info << "Deleting inconsistent processor face decomposition"
          << " map " << fName << endl;
        rm(fName);
      }
    }
  }
  if (pointProcAddressing.headerOk()) {
    pointProcAddressing.instance() = mesh.facesInstance();
    if (pointProcAddressing.size() == mesh.nPoints()) {
      pointProcAddressing.write();
    } else {
      const fileName fName(pointProcAddressing.filePath());
      if (fName.size()) {
        Info << "Deleting inconsistent processor point decomposition"
          << " map " << fName << endl;
        rm(fName);
      }
    }
  }
  if (boundaryProcAddressing.headerOk()) {
    boundaryProcAddressing.instance() = mesh.facesInstance();
    if (boundaryProcAddressing.size() == mesh.boundaryMesh().size()) {
      boundaryProcAddressing.write();
    } else {
      const fileName fName(boundaryProcAddressing.filePath());
      if (fName.size()) {
        Info << "Deleting inconsistent processor patch decomposition"
          << " map " << fName << endl;
        rm(fName);
      }
    }
  }
  if (writeMaps) {
    // For debugging: write out region
    createScalarField(mesh, "origCellID", map().cellMap())().write();
    createScalarField(mesh, "cellID", identity(mesh.nCells()))().write();
    Info << nl << "Written current cellID and origCellID as volScalarField"
      << " for use in postprocessing."
      << nl << endl;
    labelIOList
    {
      {
        "cellMap",
        mesh.facesInstance(),
        polyMesh::meshSubDir,
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      map().cellMap()
    }.write();
    labelIOList
    {
      {
        "faceMap",
        mesh.facesInstance(),
        polyMesh::meshSubDir,
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      map().faceMap()
    }.write();
    labelIOList
    {
      {
        "pointMap",
        mesh.facesInstance(),
        polyMesh::meshSubDir,
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      map().pointMap()
    }.write();
  }
  // Renumber sets if required
  if (renumberSets) {
    Info << endl;
    // Read sets
    IOobjectList objects{mesh, mesh.facesInstance(), "polyMesh/sets"};

    {
      IOobjectList cSets{objects.lookupClass(cellSet::typeName)};
      if (cSets.size()) {
        Info << "Renumbering cellSets:" << endl;
        FOR_ALL_CONST_ITER(IOobjectList, cSets, iter) {
          cellSet cs(*iter());
          Info << "    " << cs.name() << endl;
          cs.updateMesh(map());
          cs.instance() = mesh.facesInstance();
          cs.write();
        }
      }
    }
    {
      IOobjectList fSets{objects.lookupClass(faceSet::typeName)};
      if (fSets.size()) {
        Info << "Renumbering faceSets:" << endl;
        FOR_ALL_CONST_ITER(IOobjectList, fSets, iter) {
          faceSet fs(*iter());
          Info << "    " << fs.name() << endl;
          fs.updateMesh(map());
          fs.instance() = mesh.facesInstance();
          fs.write();
        }
      }
    }
    {
      IOobjectList pSets{objects.lookupClass(pointSet::typeName)};
      if (pSets.size()) {
        Info << "Renumbering pointSets:" << endl;
        FOR_ALL_CONST_ITER(IOobjectList, pSets, iter) {
          pointSet ps{*iter()};
          Info << "    " << ps.name() << endl;
          ps.updateMesh(map());
          ps.instance() = mesh.facesInstance();
          ps.write();
        }
      }
    }
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

