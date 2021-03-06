// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "fv_mesh.hpp"
#include "poly_topo_change.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "mapped_wall_poly_patch.hpp"
#include "create_shell_mesh.hpp"
#include "sync_tools.hpp"
#include "cyclic_poly_patch.hpp"
#include "wedge_poly_patch.hpp"
#include "nonuniform_transform_cyclic_poly_patch.hpp"
#include "extrude_model.hpp"
#include "global_index.hpp"
#include "face_set.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "point_fields.hpp"
#include "fv_mesh_tools.hpp"
#include "obj_stream.hpp"
#include "patch_tools.hpp"


using namespace mousse;


label findPatchID(const List<polyPatch*>& newPatches, const word& name)
{
  FOR_ALL(newPatches, i) {
    if (newPatches[i]->name() == name) {
      return i;
    }
  }
  return -1;
}


template<class PatchType>
label addPatch
(
  const polyBoundaryMesh& patches,
  const word& patchName,
  DynamicList<polyPatch*>& newPatches
)
{
  label patchI = findPatchID(newPatches, patchName);
  if (patchI != -1) {
    if (isA<PatchType>(*newPatches[patchI])) {
      // Already there
      return patchI;
    } else {
      FATAL_ERROR_IN
      (
        "addPatch<PatchType>(const polyBoundaryMesh&,"
        " const word&, DynamicList<polyPatch*>)"
      )
      << "Already have patch " << patchName
      << " but of type " << newPatches[patchI]->type()
      << exit(FatalError);
    }
  }
  patchI = newPatches.size();
  label startFaceI = 0;
  if (patchI > 0) {
    const polyPatch& pp = *newPatches.last();
    startFaceI = pp.start() + pp.size();
  }
  newPatches.append
    (
      polyPatch::New
      (
        PatchType::typeName,
        patchName,
        0,                          // size
        startFaceI,                 // nFaces
        patchI,
        patches
      ).ptr()
    );
  return patchI;
}


template<class PatchType>
label addPatch
(
  const polyBoundaryMesh& patches,
  const word& patchName,
  const dictionary& dict,
  DynamicList<polyPatch*>& newPatches
)
{
  label patchI = findPatchID(newPatches, patchName);
  if (patchI != -1) {
    if (isA<PatchType>(*newPatches[patchI])) {
      // Already there
      return patchI;
    } else {
      FATAL_ERROR_IN
      (
        "addPatch<PatchType>(const polyBoundaryMesh&,"
        " const word&, DynamicList<polyPatch*>)"
      )
      << "Already have patch " << patchName
      << " but of type " << newPatches[patchI]->type()
      << exit(FatalError);
    }
  }
  patchI = newPatches.size();
  label startFaceI = 0;
  if (patchI > 0) {
    const polyPatch& pp = *newPatches.last();
    startFaceI = pp.start()+pp.size();
  }
  dictionary patchDict{dict};
  patchDict.set("type", PatchType::typeName);
  patchDict.set("nFaces", 0);
  patchDict.set("startFace", startFaceI);
  newPatches.append
    (
      polyPatch::New
      (
        patchName,
        patchDict,
        patchI,
        patches
      ).ptr()
    );
  return patchI;
}


// Remove zero-sized patches
void deleteEmptyPatches(fvMesh& mesh)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  wordList masterNames;
  if (Pstream::master()) {
    masterNames = patches.names();
  }
  Pstream::scatter(masterNames);
  labelList oldToNew{patches.size(), -1};
  label usedI = 0;
  label notUsedI = patches.size();
  // Add all the non-empty, non-processor patches
  FOR_ALL(masterNames, masterI) {
    label patchI = patches.findPatchID(masterNames[masterI]);
    if (patchI == -1)
      continue;
    if (isA<processorPolyPatch>(patches[patchI])) {
      // Similar named processor patch? Not 'possible'.
      if (patches[patchI].size() == 0) {
        Pout << "Deleting processor patch " << patchI
          << " name:" << patches[patchI].name()
          << endl;
        oldToNew[patchI] = --notUsedI;
      } else {
        oldToNew[patchI] = usedI++;
      }
    } else {
      // Common patch.
      if (returnReduce(patches[patchI].size(), sumOp<label>()) == 0) {
        Pout << "Deleting patch " << patchI
          << " name:" << patches[patchI].name()
          << endl;
        oldToNew[patchI] = --notUsedI;
      } else {
        oldToNew[patchI] = usedI++;
      }
    }
  }
  // Add remaining patches at the end
  FOR_ALL(patches, patchI) {
    if (oldToNew[patchI] != -1)
      continue;
    // Unique to this processor. Note: could check that these are
    // only processor patches.
    if (patches[patchI].size() == 0) {
      Pout << "Deleting processor patch " << patchI
        << " name:" << patches[patchI].name()
        << endl;
      oldToNew[patchI] = --notUsedI;
    } else {
      oldToNew[patchI] = usedI++;
    }
  }
  fvMeshTools::reorderPatches(mesh, oldToNew, usedI, true);
}


void createDummyFvMeshFiles(const polyMesh& mesh, const word& regionName)
{
  // Create dummy system/fv*
  {
    IOobject io
    {
      "fvSchemes",
      mesh.time().system(),
      regionName,
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    };
    Info << "Testing:" << io.objectPath() << endl;
    if (!io.headerOk()) {
      Info << "Writing dummy " << regionName/io.name() << endl;
      dictionary dummyDict;
      dictionary divDict;
      dummyDict.add("divSchemes", divDict);
      dictionary gradDict;
      dummyDict.add("gradSchemes", gradDict);
      dictionary laplDict;
      dummyDict.add("laplacianSchemes", laplDict);
      IOdictionary(io, dummyDict).regIOobject::write();
    }
  }
  {
    IOobject io
    {
      "fvSolution",
      mesh.time().system(),
      regionName,
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    };
    if (!io.headerOk()) {
      Info << "Writing dummy " << regionName/io.name() << endl;
      dictionary dummyDict;
      IOdictionary(io, dummyDict).regIOobject::write();
    }
  }
}


// Check zone either all internal or all external faces
void checkZoneInside
(
  const polyMesh& mesh,
  const wordList& zoneNames,
  const labelList& zoneID,
  const labelList& extrudeMeshFaces,
  const boolList& isInternal
)
{
  FOR_ALL(zoneNames, i) {
    if (isInternal[i]) {
      Info<< "Zone " << zoneNames[i] << " has internal faces" << endl;
    } else {
      Info<< "Zone " << zoneNames[i] << " has boundary faces" << endl;
    }
  }
  FOR_ALL(extrudeMeshFaces, i) {
    label faceI = extrudeMeshFaces[i];
    label zoneI = zoneID[i];
    if (isInternal[zoneI] != mesh.isInternalFace(faceI)) {
      FATAL_ERROR_IN("checkZoneInside(..)")
        << "Zone " << zoneNames[zoneI]
        << " is not consistently all internal or all boundary faces."
        << " Face " << faceI << " at " << mesh.faceCentres()[faceI]
        << " is the first occurrence."
        << exit(FatalError);
    }
  }
}


// To combineReduce a labelList. Filters out duplicates.
class uniqueEqOp
{
public:
  void operator()(labelList& x, const labelList& y) const
  {
    if (x.empty()) {
      if (y.size()) {
        x = y;
      }
    } else {
      FOR_ALL(y, yi) {
        if (findIndex(x, y[yi]) == -1) {
          label sz = x.size();
          x.setSize(sz+1);
          x[sz] = y[yi];
        }
      }
    }
  }
};


// Calculate global pp faces per pp edge.
labelListList globalEdgeFaces
(
  const polyMesh& mesh,
  const globalIndex& globalFaces,
  const primitiveFacePatch& pp,
  const labelList& ppMeshEdges
)
{
  // From mesh edge to global pp face labels.
  labelListList globalEdgeFaces{ppMeshEdges.size()};
  const labelListList& edgeFaces = pp.edgeFaces();
  FOR_ALL(edgeFaces, edgeI) {
    const labelList& eFaces = edgeFaces[edgeI];
    // Store pp face and processor as unique tag.
    labelList& globalEFaces = globalEdgeFaces[edgeI];
    globalEFaces.setSize(eFaces.size());
    FOR_ALL(eFaces, i) {
      globalEFaces[i] = globalFaces.toGlobal(eFaces[i]);
    }
  }
  // Synchronise across coupled edges.
  syncTools::syncEdgeList
    (
      mesh,
      ppMeshEdges,
      globalEdgeFaces,
      uniqueEqOp(),
      labelList()             // null value
    );
  return globalEdgeFaces;
}


// Find a patch face that is not extruded. Return -1 if not found.
label findUncoveredPatchFace
(
  const fvMesh& mesh,
  const UIndirectList<label>& extrudeMeshFaces,// mesh faces that are extruded
  const label meshEdgeI                       // mesh edge
)
{
  // Make set of extruded faces.
  labelHashSet extrudeFaceSet{extrudeMeshFaces.size()};
  FOR_ALL(extrudeMeshFaces, i) {
    extrudeFaceSet.insert(extrudeMeshFaces[i]);
  }
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  const labelList& eFaces = mesh.edgeFaces()[meshEdgeI];
  FOR_ALL(eFaces, i) {
    label fI = eFaces[i];
    label pI = pbm.whichPatch(fI);
    if (pI != -1 && !pbm[pI].coupled() && !extrudeFaceSet.found(fI)) {
      return fI;
    }
  }
  return -1;
}


// Same as findUncoveredPatchFace, except explicitly checks for cyclic faces
label findUncoveredCyclicPatchFace
(
  const fvMesh& mesh,
  const UIndirectList<label>& extrudeMeshFaces,// mesh faces that are extruded
  const label meshEdgeI                       // mesh edge
)
{
  // Make set of extruded faces.
  labelHashSet extrudeFaceSet{extrudeMeshFaces.size()};
  FOR_ALL(extrudeMeshFaces, i) {
    extrudeFaceSet.insert(extrudeMeshFaces[i]);
  }
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  const labelList& eFaces = mesh.edgeFaces()[meshEdgeI];
  FOR_ALL(eFaces, i) {
    label fI = eFaces[i];
    label pI = pbm.whichPatch(fI);
    if (pI != -1 && isA<cyclicPolyPatch>(pbm[pI])
        && !extrudeFaceSet.found(fI)) {
      return fI;
    }
  }
  return -1;
}


// Calculate per edge min and max zone
void calcEdgeMinMaxZone
(
  const fvMesh& mesh,
  const primitiveFacePatch& /*extrudePatch*/,
  const labelList& extrudeMeshEdges,
  const labelList& zoneID,
  const mapDistribute& extrudeEdgeFacesMap,
  const labelListList& extrudeEdgeGlobalFaces,
  labelList& minZoneID,
  labelList& maxZoneID
)
{
  // Get zoneIDs in extrudeEdgeGlobalFaces order
  labelList mappedZoneID{zoneID};
  extrudeEdgeFacesMap.distribute(mappedZoneID);
  // Get min and max zone per edge
  minZoneID.setSize(extrudeEdgeGlobalFaces.size(), labelMax);
  maxZoneID.setSize(extrudeEdgeGlobalFaces.size(), labelMin);
  FOR_ALL(extrudeEdgeGlobalFaces, edgeI) {
    const labelList& eFaces = extrudeEdgeGlobalFaces[edgeI];
    if (eFaces.size()) {
      FOR_ALL(eFaces, i) {
        label zoneI = mappedZoneID[eFaces[i]];
        minZoneID[edgeI] = min(minZoneID[edgeI], zoneI);
        maxZoneID[edgeI] = max(maxZoneID[edgeI], zoneI);
      }
    }
  }
  syncTools::syncEdgeList
    (
      mesh,
      extrudeMeshEdges,
      minZoneID,
      minEqOp<label>(),
      labelMax        // null value
    );
  syncTools::syncEdgeList
    (
      mesh,
      extrudeMeshEdges,
      maxZoneID,
      maxEqOp<label>(),
      labelMin        // null value
    );
}


// Count the number of faces in patches that need to be created. Calculates:
//  zoneSidePatch[zoneI]         : the number of side faces to be created
//  zoneZonePatch[zoneA,zoneB]   : the number of faces inbetween zoneA and B
// Since this only counts we're not taking the processor patches into
// account.
void countExtrudePatches
(
  const fvMesh& mesh,
  const label nZones,
  const primitiveFacePatch& extrudePatch,
  const labelList& extrudeMeshFaces,
  const labelList& extrudeMeshEdges,
  const labelListList& extrudeEdgeGlobalFaces,
  const labelList& minZoneID,
  const labelList& maxZoneID,
  labelList& zoneSidePatch,
  labelList& zoneZonePatch
)
{
  // Check on master edge for use of zones. Since we only want to know
  // whether they are being used at all no need to accurately count on slave
  // edge as well. Just add all together at the end of this routine so it
  // gets detected at least.
  FOR_ALL(extrudePatch.edgeFaces(), edgeI) {
    const labelList& eFaces = extrudePatch.edgeFaces()[edgeI];
    if (eFaces.size() == 2) {
      // Internal edge - check if inbetween different zones.
      if (minZoneID[edgeI] != maxZoneID[edgeI]) {
        zoneZonePatch[minZoneID[edgeI]*nZones+maxZoneID[edgeI]]++;
      }
    } else if (eFaces.size() == 1 && extrudeEdgeGlobalFaces[edgeI].size() == 2) {
      // Coupled edge - check if inbetween different zones.
      if (minZoneID[edgeI] != maxZoneID[edgeI]) {
        const edge& e = extrudePatch.edges()[edgeI];
        const pointField& pts = extrudePatch.localPoints();
        WARNING_IN("countExtrudePatches(..)")
          << "Edge " << edgeI
          << "at " << pts[e[0]] << pts[e[1]]
          << " is a coupled edge and inbetween two different zones "
          << minZoneID[edgeI] << " and " << maxZoneID[edgeI] << endl
          << "    This is currently not supported." << endl;
        zoneZonePatch[minZoneID[edgeI]*nZones+maxZoneID[edgeI]]++;
      }
    } else {
      // One or more than two edge-faces.
      // Check whether we are on a mesh edge with external patches. If
      // so choose any uncovered one. If none found put face in
      // undetermined zone 'side' patch
      label faceI =
        findUncoveredPatchFace
        (
          mesh,
          UIndirectList<label>(extrudeMeshFaces, eFaces),
          extrudeMeshEdges[edgeI]
        );
      if (faceI == -1) {
        zoneSidePatch[minZoneID[edgeI]]++;
      }
    }
  }
  // Synchronise decistion. Actual numbers are not important, just make
  // sure that they're > 0 on all processors.
  Pstream::listCombineGather(zoneSidePatch, plusEqOp<label>());
  Pstream::listCombineScatter(zoneSidePatch);
  Pstream::listCombineGather(zoneZonePatch, plusEqOp<label>());
  Pstream::listCombineScatter(zoneZonePatch);
}


void addCouplingPatches
(
  const fvMesh& mesh,
  const word& regionName,
  const word& shellRegionName,
  const wordList& zoneNames,
  const wordList& zoneShadowNames,
  const boolList& isInternal,
  const labelList& /*zoneIDs*/,
  DynamicList<polyPatch*>& newPatches,
  labelList& interRegionTopPatch,
  labelList& interRegionBottomPatch
)
{
  Pout << "Adding coupling patches:" << nl << nl
    << "patchID\tpatch\ttype" << nl
    << "-------\t-----\t----"
    << endl;
  interRegionTopPatch.setSize(zoneNames.size(), -1);
  interRegionBottomPatch.setSize(zoneNames.size(), -1);
  label nOldPatches = newPatches.size();
  FOR_ALL(zoneNames, zoneI) {
    word interName
    {
      regionName +"_to_" +shellRegionName +'_' + zoneNames[zoneI]
    };
    if (isInternal[zoneI]) {
      interRegionTopPatch[zoneI] =
        addPatch<mappedWallPolyPatch>
        (
          mesh.boundaryMesh(),
          interName + "_top",
          newPatches
        );
      Pout << interRegionTopPatch[zoneI]
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->type()
        << nl;
      interRegionBottomPatch[zoneI] =
        addPatch<mappedWallPolyPatch>
        (
          mesh.boundaryMesh(),
          interName + "_bottom",
          newPatches
        );
      Pout << interRegionBottomPatch[zoneI]
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->type()
        << nl;
    } else if (zoneShadowNames.size() == 0) {
      interRegionTopPatch[zoneI] =
        addPatch<polyPatch>
        (
          mesh.boundaryMesh(),
          zoneNames[zoneI] + "_top",
          newPatches
        );
      Pout << interRegionTopPatch[zoneI]
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->type()
        << nl;
      interRegionBottomPatch[zoneI] =
        addPatch<mappedWallPolyPatch>
        (
          mesh.boundaryMesh(),
          interName,
          newPatches
        );
      Pout << interRegionBottomPatch[zoneI]
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->type()
        << nl;
    } else {   //patch using shadow face zones.
      interRegionTopPatch[zoneI] =
        addPatch<mappedWallPolyPatch>
        (
          mesh.boundaryMesh(),
          zoneShadowNames[zoneI] + "_top",
          newPatches
        );
      Pout << interRegionTopPatch[zoneI]
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionTopPatch[zoneI]]->type()
        << nl;
      interRegionBottomPatch[zoneI] =
        addPatch<mappedWallPolyPatch>
        (
          mesh.boundaryMesh(),
          interName,
          newPatches
        );
      Pout << interRegionBottomPatch[zoneI]
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->name()
        << '\t' << newPatches[interRegionBottomPatch[zoneI]]->type()
        << nl;
    }
  }
  Pout << "Added " << newPatches.size()-nOldPatches
    << " inter-region patches." << nl
    << endl;
}


// Sets sidePatch[edgeI] to interprocessor or cyclic patch. Adds any
// coupled patches if necessary.
void addCoupledPatches
(
  const fvMesh& mesh,
  const primitiveFacePatch& extrudePatch,
  const labelList& extrudeMeshFaces,
  const labelList& extrudeMeshEdges,
  const mapDistribute& extrudeEdgeFacesMap,
  const labelListList& extrudeEdgeGlobalFaces,
  labelList& sidePatchID,
  DynamicList<polyPatch*>& newPatches
)
{
  // Calculate opposite processor for coupled edges (only if shared by
  // two procs). Note: could have saved original globalEdgeFaces structure.
  // Get procID in extrudeEdgeGlobalFaces order
  labelList procID{extrudeEdgeGlobalFaces.size(), Pstream::myProcNo()};
  extrudeEdgeFacesMap.distribute(procID);
  labelList minProcID{extrudeEdgeGlobalFaces.size(), labelMax};
  labelList maxProcID{extrudeEdgeGlobalFaces.size(), labelMin};
  FOR_ALL(extrudeEdgeGlobalFaces, edgeI) {
    const labelList& eFaces = extrudeEdgeGlobalFaces[edgeI];
    if (!eFaces.size())
      continue;
    FOR_ALL(eFaces, i) {
      label procI = procID[eFaces[i]];
      minProcID[edgeI] = min(minProcID[edgeI], procI);
      maxProcID[edgeI] = max(maxProcID[edgeI], procI);
    }
  }
  syncTools::syncEdgeList
    (
      mesh,
      extrudeMeshEdges,
      minProcID,
      minEqOp<label>(),
      labelMax        // null value
    );
  syncTools::syncEdgeList
    (
      mesh,
      extrudeMeshEdges,
      maxProcID,
      maxEqOp<label>(),
      labelMin        // null value
    );
  Pout << "Adding processor or cyclic patches:" << nl << nl
    << "patchID\tpatch" << nl
    << "-------\t-----"
    << endl;
  label nOldPatches = newPatches.size();
  sidePatchID.setSize(extrudePatch.edgeFaces().size(), -1);
  FOR_ALL(extrudePatch.edgeFaces(), edgeI) {
    const labelList& eFaces = extrudePatch.edgeFaces()[edgeI];
    if (eFaces.size() == 1 && extrudeEdgeGlobalFaces[edgeI].size() == 2) {
      // coupled boundary edge. Find matching patch.
      label nbrProcI = minProcID[edgeI];
      if (nbrProcI == Pstream::myProcNo()) {
        nbrProcI = maxProcID[edgeI];
      }
      if (nbrProcI == Pstream::myProcNo()) {
        // Cyclic patch since both procs the same. This cyclic should
        // already exist in newPatches so no adding necessary.
        label faceI =
          findUncoveredCyclicPatchFace
          (
            mesh,
            UIndirectList<label>{extrudeMeshFaces, eFaces},
            extrudeMeshEdges[edgeI]
          );
        if (faceI != -1) {
          const polyBoundaryMesh& patches = mesh.boundaryMesh();
          label newPatchI =
            findPatchID
            (
              newPatches,
              patches[patches.whichPatch(faceI)].name()
            );
          sidePatchID[edgeI] = newPatchI;
        } else {
          FATAL_ERROR_IN
          (
            "void addCoupledPatches"
            "("
            "  const fvMesh&, "
            "  const primitiveFacePatch&, "
            "  const labelList&, "
            "  const labelList&, "
            "  const mapDistribute&, "
            "  const labelListList&, "
            "  labelList&, "
            "  DynamicList<polyPatch*>&"
            ")"
          )
          << "Unable to determine coupled patch addressing"
          << abort(FatalError);
        }
      } else {
        // Rrocessor patch
        const auto me = mousse::name(Pstream::myProcNo());
        const auto other = mousse::name(nbrProcI);
        word name = "procBoundary" + me + "to" + other;
        sidePatchID[edgeI] = findPatchID(newPatches, name);
        if (sidePatchID[edgeI] == -1) {
          dictionary patchDict;
          patchDict.add("myProcNo", Pstream::myProcNo());
          patchDict.add("neighbProcNo", nbrProcI);
          sidePatchID[edgeI] =
            addPatch<processorPolyPatch>
            (
              mesh.boundaryMesh(),
              name,
              patchDict,
              newPatches
            );
          Pout << sidePatchID[edgeI] << '\t' << name << nl;
        }
      }
    }
  }
  Pout << "Added " << newPatches.size()-nOldPatches
    << " coupled patches." << nl
    << endl;
}


void addZoneSidePatches
(
  const fvMesh& mesh,
  const wordList& zoneNames,
  const word& oneDPolyPatchType,
  DynamicList<polyPatch*>& newPatches,
  labelList& zoneSidePatch
)
{
  Pout << "Adding patches for sides on zones:" << nl << nl
    << "patchID\tpatch" << nl
    << "-------\t-----"
    << endl;
  label nOldPatches = newPatches.size();
  FOR_ALL(zoneSidePatch, zoneI) {
    if (oneDPolyPatchType != word::null) {
      // Reuse single empty patch.
      word patchName;
      if (oneDPolyPatchType == "empty") {
        patchName = "oneDEmptyPatch";
        zoneSidePatch[zoneI] =
          addPatch<emptyPolyPatch>
          (
            mesh.boundaryMesh(),
            patchName,
            newPatches
          );
      } else if (oneDPolyPatchType == "wedge") {
        patchName = "oneDWedgePatch";
        zoneSidePatch[zoneI] =
          addPatch<wedgePolyPatch>
          (
            mesh.boundaryMesh(),
            patchName,
            newPatches
          );
      } else {
        FATAL_ERROR_IN("addZoneSidePatches()")
          << "Type " << oneDPolyPatchType << " does not exist "
          << exit(FatalError);
      }
      Pout << zoneSidePatch[zoneI] << '\t' << patchName << nl;
    } else if (zoneSidePatch[zoneI] > 0) {
      word patchName = zoneNames[zoneI] + "_" + "side";
      zoneSidePatch[zoneI] =
        addPatch<polyPatch>
        (
          mesh.boundaryMesh(),
          patchName,
          newPatches
        );
      Pout << zoneSidePatch[zoneI] << '\t' << patchName << nl;
    }
  }
  Pout << "Added " << newPatches.size() - nOldPatches << " zone-side patches."
    << nl << endl;
}


void addInterZonePatches
(
  const fvMesh& mesh,
  const wordList& zoneNames,
  const bool oneD,
  labelList& zoneZonePatch_min,
  labelList& zoneZonePatch_max,
  DynamicList<polyPatch*>& newPatches
)
{
  Pout << "Adding inter-zone patches:" << nl << nl
    << "patchID\tpatch" << nl
    << "-------\t-----"
    << endl;
  dictionary transformDict;
  transformDict.add
    (
      "transform",
      cyclicPolyPatch::transformTypeNames[cyclicPolyPatch::NOORDERING]
    );
  label nOldPatches = newPatches.size();
  if (!oneD) {
    FOR_ALL(zoneZonePatch_min, minZone) {
      for (label maxZone = minZone; maxZone < zoneNames.size(); maxZone++) {
        label index = minZone*zoneNames.size() + maxZone;
        if (zoneZonePatch_min[index] <= 0)
          continue;
        word minToMax =
          zoneNames[minZone] + "_to_" + zoneNames[maxZone];
        word maxToMin =
          zoneNames[maxZone] + "_to_" + zoneNames[minZone];

        {
          transformDict.set("neighbourPatch", maxToMin);
          zoneZonePatch_min[index] =
            addPatch<nonuniformTransformCyclicPolyPatch>
            (
              mesh.boundaryMesh(),
              minToMax,
              transformDict,
              newPatches
            );
          Pout << zoneZonePatch_min[index] << '\t' << minToMax << nl;
        }

        {
          transformDict.set("neighbourPatch", minToMax);
          zoneZonePatch_max[index] =
            addPatch<nonuniformTransformCyclicPolyPatch>
            (
              mesh.boundaryMesh(),
              maxToMin,
              transformDict,
              newPatches
            );
          Pout << zoneZonePatch_max[index] << '\t' << maxToMin << nl;
        }

      }
    }
  }
  Pout << "Added " << newPatches.size() - nOldPatches << " inter-zone patches."
    << nl << endl;
}


tmp<pointField> calcOffset
(
  const primitiveFacePatch& extrudePatch,
  const createShellMesh& extruder,
  const polyPatch& pp
)
{
  vectorField::subField fc = pp.faceCentres();
  tmp<pointField> toffsets{new pointField{fc.size()}};
  pointField& offsets = toffsets();
  FOR_ALL(fc, i) {
    label meshFaceI = pp.start()+i;
    label patchFaceI = mag(extruder.faceToFaceMap()[meshFaceI])-1;
    point patchFc =
      extrudePatch[patchFaceI].centre
      (
        extrudePatch.points()
      );
    offsets[i] = patchFc - fc[i];
  }
  return toffsets;
}


void setCouplingInfo
(
  fvMesh& mesh,
  const labelList& zoneToPatch,
  const word& sampleRegion,
  const mappedWallPolyPatch::sampleMode mode,
  const List<pointField>& offsets
)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  List<polyPatch*> newPatches{patches.size(), static_cast<polyPatch*>(nullptr)};
  FOR_ALL(zoneToPatch, zoneI) {
    label patchI = zoneToPatch[zoneI];
    if (patchI == -1)
      continue;
    const polyPatch& pp = patches[patchI];
    if (!isA<mappedWallPolyPatch>(pp))
      continue;
    newPatches[patchI] =
      new mappedWallPolyPatch
      {
        pp.name(),
        pp.size(),
        pp.start(),
        patchI,
        sampleRegion,                           // sampleRegion
        mode,                                   // sampleMode
        pp.name(),                              // samplePatch
        offsets[zoneI],                         // offset
        patches
      };
  }
  FOR_ALL(newPatches, patchI) {
    if (!newPatches[patchI]) {
      newPatches[patchI] = patches[patchI].clone(patches).ptr();
    }
  }
  mesh.removeFvBoundary();
  mesh.addFvPatches(newPatches, true);
}


// Extrude and write geometric properties
void extrudeGeometricProperties
(
  const polyMesh& mesh,
  const primitiveFacePatch& extrudePatch,
  const createShellMesh& extruder,
  const polyMesh& regionMesh,
  const extrudeModel& model
)
{
  const pointIOField patchFaceCentres
  {
    // IOobject
    {
      "patchFaceCentres",
      mesh.pointsInstance(),
      mesh.meshSubDir,
      mesh,
      IOobject::MUST_READ
    }
  };
  const pointIOField patchEdgeCentres
  {
    // IOobject
    {
      "patchEdgeCentres",
      mesh.pointsInstance(),
      mesh.meshSubDir,
      mesh,
      IOobject::MUST_READ
    }
  };
  // Determine edge normals on original patch
  labelList patchEdges;
  labelList coupledEdges;
  PackedBoolList sameEdgeOrientation;
  PatchTools::matchEdges
    (
      extrudePatch,
      mesh.globalData().coupledPatch(),
      patchEdges,
      coupledEdges,
      sameEdgeOrientation
    );
  pointField patchEdgeNormals
  {
    PatchTools::edgeNormals
    (
      mesh,
      extrudePatch,
      patchEdges,
      coupledEdges
    )
  };
  pointIOField faceCentres
  {
    // IOobject
    {
      "faceCentres",
      regionMesh.pointsInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    regionMesh.nFaces()
  };
  // Work out layers. Guaranteed in columns so no fancy parallel bits.
  FOR_ALL(extruder.faceToFaceMap(), faceI) {
    if (extruder.faceToFaceMap()[faceI] != 0) {
      // 'horizontal' face
      label patchFaceI = mag(extruder.faceToFaceMap()[faceI])-1;
      label cellI = regionMesh.faceOwner()[faceI];
      if (regionMesh.isInternalFace(faceI)) {
        cellI = max(cellI, regionMesh.faceNeighbour()[faceI]);
      }
      // Calculate layer from cell numbering (see createShellMesh)
      label layerI = (cellI % model.nLayers());
      if (!regionMesh.isInternalFace(faceI)
          && extruder.faceToFaceMap()[faceI] > 0) {
        // Top face
        layerI++;
      }
      // Recalculate based on extrusion model
      faceCentres[faceI] =
        model
        (
          patchFaceCentres[patchFaceI],
          extrudePatch.faceNormals()[patchFaceI],
          layerI
        );
    } else {
      // 'vertical face
      label patchEdgeI = extruder.faceToEdgeMap()[faceI];
      label layerI = regionMesh.faceOwner()[faceI] % model.nLayers();
      // Extrude patch edge centre to this layer
      point pt0 =
        model
        (
          patchEdgeCentres[patchEdgeI],
          patchEdgeNormals[patchEdgeI],
          layerI
        );
      // Extrude patch edge centre to next layer
      point pt1 =
        model
        (
          patchEdgeCentres[patchEdgeI],
          patchEdgeNormals[patchEdgeI],
          layerI+1
        );
      // Interpolate
      faceCentres[faceI] = 0.5*(pt0+pt1);
    }
  }
  pointIOField cellCentres
  {
    // IOobject
    {
      "cellCentres",
      regionMesh.pointsInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    regionMesh.nCells()
  };
  FOR_ALL(extruder.cellToFaceMap(), cellI) {
    label patchFaceI = extruder.cellToFaceMap()[cellI];
    // Calculate layer from cell numbering (see createShellMesh)
    label layerI = (cellI % model.nLayers());
    // Recalculate based on extrusion model
    point pt0 =
      model
      (
        patchFaceCentres[patchFaceI],
        extrudePatch.faceNormals()[patchFaceI],
        layerI
      );
    point pt1 =
      model
      (
        patchFaceCentres[patchFaceI],
        extrudePatch.faceNormals()[patchFaceI],
        layerI+1
      );
    // Interpolate
    cellCentres[cellI] = 0.5*(pt0+pt1);
  }
  // Bit of checking
  if (false) {
    OBJstream faceStr{regionMesh.time().path()/"faceCentres.obj"};
    OBJstream cellStr{regionMesh.time().path()/"cellCentres.obj"};
    FOR_ALL(faceCentres, faceI) {
      Pout << "Model     :" << faceCentres[faceI] << endl
        << "regionMesh:" << regionMesh.faceCentres()[faceI] << endl;
      faceStr.write
      (
        linePointRef{faceCentres[faceI], regionMesh.faceCentres()[faceI]}
      );
    }
    FOR_ALL(cellCentres, cellI) {
      Pout << "Model     :" << cellCentres[cellI] << endl
        << "regionMesh:" << regionMesh.cellCentres()[cellI] << endl;
      cellStr.write
      (
        linePointRef{cellCentres[cellI], regionMesh.cellCentres()[cellI]}
      );
    }
  }
  Info << "Writing geometric properties for mesh " << regionMesh.name()
    << " to " << regionMesh.pointsInstance() << nl
    << endl;
  bool ok = faceCentres.write() && cellCentres.write();
  if (!ok) {
    FATAL_ERROR_IN("extrudeGeometricProperties(..)")
      << "Failed writing " << faceCentres.objectPath()
      << " and " << cellCentres.objectPath()
      << exit(FatalError);
  }
}


int main(int argc, char *argv[])
{
  argList::addNote("Create region mesh by extruding a faceZone or faceSet");
  #include "add_region_option.inc"
  #include "add_overwrite_option.inc"
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_mesh.inc"
  if (mesh.boundaryMesh().checkParallelSync(true)) {
    List<wordList> allNames{Pstream::nProcs()};
    allNames[Pstream::myProcNo()] = mesh.boundaryMesh().names();
    Pstream::gatherList(allNames);
    Pstream::scatterList(allNames);
    FATAL_ERROR_IN(args.executable())
      << "Patches are not synchronised on all processors."
      << " Per processor patches " << allNames
      << exit(FatalError);
  }
  const word oldInstance = mesh.pointsInstance();
  bool overwrite = args.optionFound("overwrite");
  const word dictName{"extrudeToRegionMeshDict"};
  #include "set_system_mesh_dictionary_io.inc"
  IOdictionary dict{dictIO};
  // Point generator
  autoPtr<extrudeModel> model{extrudeModel::New(dict)};
  // Region
  const word shellRegionName{dict.lookup("region")};
  // Faces to extrude - either faceZones or faceSets (boundary faces only)
  wordList zoneNames;
  wordList zoneShadowNames;
  bool hasZones = dict.found("faceZones");
  if (hasZones) {
    dict.lookup("faceZones") >> zoneNames;
    dict.readIfPresent("faceZonesShadow", zoneShadowNames);
    // Check
    if (dict.found("faceSets")) {
      FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
        << "Please supply faces to extrude either through 'faceZones'"
        << " or 'faceSets' entry. Found both."
        << exit(FatalIOError);
    }
  } else {
    dict.lookup("faceSets") >> zoneNames;
    dict.readIfPresent("faceSetsShadow", zoneShadowNames);
  }
  mappedPatchBase::sampleMode sampleMode =
    mappedPatchBase::sampleModeNames_[dict.lookup("sampleMode")];
  const Switch oneD{dict.lookup("oneD")};
  Switch oneDNonManifoldEdges{false};
  word oneDPatchType{emptyPolyPatch::typeName};
  if (oneD) {
    oneDNonManifoldEdges = dict.lookupOrDefault("nonManifold", false);
    dict.lookup("oneDPolyPatchType") >> oneDPatchType;
  }
  const Switch adaptMesh{dict.lookup("adaptMesh")};
  if (hasZones) {
    Info << "Extruding zones " << zoneNames
      << " on mesh " << regionName
      << " into shell mesh " << shellRegionName
      << endl;
  } else {
    Info << "Extruding faceSets " << zoneNames
      << " on mesh " << regionName
      << " into shell mesh " << shellRegionName
      << endl;
  }
  if (shellRegionName == regionName) {
    FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
      << "Cannot extrude into same region as mesh." << endl
      << "Mesh region : " << regionName << endl
      << "Shell region : " << shellRegionName
      << exit(FatalIOError);
  }
  if (oneD) {
    if (oneDNonManifoldEdges) {
      Info << "Extruding as 1D columns with sides in patch type "
        << oneDPatchType
        << " and connected points (except on non-manifold areas)."
        << endl;
    } else {
      Info << "Extruding as 1D columns with sides in patch type "
        << oneDPatchType
        << " and duplicated points (overlapping volumes)."
        << endl;
    }
  }
  // Create dummy fv* files
  createDummyFvMeshFiles(mesh, shellRegionName);
  word meshInstance;
  if (!overwrite) {
    runTime++;
    meshInstance = runTime.timeName();
  } else {
    meshInstance = oldInstance;
  }
  Info << "Writing meshes to " << meshInstance << nl << endl;
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Extract faces to extrude
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  // Note: zoneID are regions of extrusion. They are not mesh.faceZones
  // indices.
  // From extrude zone to mesh zone (or -1 if extruding faceSets)
  labelList meshZoneID;
  // Per extrude zone whether contains internal or external faces
  boolList isInternal{zoneNames.size(), false};
  labelList extrudeMeshFaces;
  faceList zoneFaces;
  labelList zoneID;
  boolList zoneFlipMap;
  // Shadow
  labelList zoneShadowIDs;    // from extrude shadow zone to mesh zone
  labelList extrudeMeshShadowFaces;
  boolList zoneShadowFlipMap;
  labelList zoneShadowID;
  if (hasZones) {
    const faceZoneMesh& faceZones = mesh.faceZones();
    meshZoneID.setSize(zoneNames.size());
    FOR_ALL(zoneNames, i) {
      meshZoneID[i] = faceZones.findZoneID(zoneNames[i]);
      if (meshZoneID[i] == -1) {
        FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
          << "Cannot find zone " << zoneNames[i] << endl
          << "Valid zones are " << faceZones.names()
          << exit(FatalIOError);
      }
    }
    // Collect per face information
    label nExtrudeFaces = 0;
    FOR_ALL(meshZoneID, i) {
      nExtrudeFaces += faceZones[meshZoneID[i]].size();
    }
    extrudeMeshFaces.setSize(nExtrudeFaces);
    zoneFaces.setSize(nExtrudeFaces);
    zoneID.setSize(nExtrudeFaces);
    zoneFlipMap.setSize(nExtrudeFaces);
    nExtrudeFaces = 0;
    FOR_ALL(meshZoneID, i) {
      const faceZone& fz = faceZones[meshZoneID[i]];
      const primitiveFacePatch& fzp = fz();
      FOR_ALL(fz, j) {
        extrudeMeshFaces[nExtrudeFaces] = fz[j];
        zoneFaces[nExtrudeFaces] = fzp[j];
        zoneID[nExtrudeFaces] = i;
        zoneFlipMap[nExtrudeFaces] = fz.flipMap()[j];
        nExtrudeFaces++;
        if (mesh.isInternalFace(fz[j])) {
          isInternal[i] = true;
        }
      }
    }
    // Shadow zone
    // ~~~~~~~~~~~
    if (zoneShadowNames.size()) {
      zoneShadowIDs.setSize(zoneShadowNames.size());
      FOR_ALL(zoneShadowNames, i) {
        zoneShadowIDs[i] = faceZones.findZoneID(zoneShadowNames[i]);
        if (zoneShadowIDs[i] == -1) {
          FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
            << "Cannot find zone " << zoneShadowNames[i] << endl
            << "Valid zones are " << faceZones.names()
            << exit(FatalIOError);
        }
      }
      label nShadowFaces = 0;
      FOR_ALL(zoneShadowIDs, i) {
        nShadowFaces += faceZones[zoneShadowIDs[i]].size();
      }
      extrudeMeshShadowFaces.setSize(nShadowFaces);
      zoneShadowFlipMap.setSize(nShadowFaces);
      zoneShadowID.setSize(nShadowFaces);
      nShadowFaces = 0;
      FOR_ALL(zoneShadowIDs, i) {
        const faceZone& fz = faceZones[zoneShadowIDs[i]];
        FOR_ALL(fz, j) {
          extrudeMeshShadowFaces[nShadowFaces] = fz[j];
          zoneShadowFlipMap[nShadowFaces] = fz.flipMap()[j];
          zoneShadowID[nShadowFaces] = i;
          nShadowFaces++;
        }
      }
    }
  } else {
    meshZoneID.setSize(zoneNames.size(), -1);
    // Load faceSets
    PtrList<faceSet> zones{zoneNames.size()};
    FOR_ALL(zoneNames, i) {
      Info << "Loading faceSet " << zoneNames[i] << endl;
      zones.set(i, new faceSet{mesh, zoneNames[i]});
    }
    // Collect per face information
    label nExtrudeFaces = 0;
    FOR_ALL(zones, i) {
      nExtrudeFaces += zones[i].size();
    }
    extrudeMeshFaces.setSize(nExtrudeFaces);
    zoneFaces.setSize(nExtrudeFaces);
    zoneID.setSize(nExtrudeFaces);
    zoneFlipMap.setSize(nExtrudeFaces);
    nExtrudeFaces = 0;
    FOR_ALL(zones, i) {
      const faceSet& fz = zones[i];
      FOR_ALL_CONST_ITER(faceSet, fz, iter) {
        label faceI = iter.key();
        if (mesh.isInternalFace(faceI)) {
          FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
            << "faceSet " << fz.name()
            << "contains internal faces."
            << " This is not permitted."
            << exit(FatalIOError);
        }
        extrudeMeshFaces[nExtrudeFaces] = faceI;
        zoneFaces[nExtrudeFaces] = mesh.faces()[faceI];
        zoneID[nExtrudeFaces] = i;
        zoneFlipMap[nExtrudeFaces] = false;
        nExtrudeFaces++;
        if (mesh.isInternalFace(faceI)) {
          isInternal[i] = true;
        }
      }
    }
    // Shadow zone
    PtrList<faceSet> shadowZones{zoneShadowNames.size()};
    if (zoneShadowNames.size()) {
      zoneShadowIDs.setSize(zoneShadowNames.size(), -1);
      FOR_ALL(zoneShadowNames, i) {
        shadowZones.set(i, new faceSet(mesh, zoneShadowNames[i]));
      }
      label nShadowFaces = 0;
      FOR_ALL(shadowZones, i) {
        nShadowFaces += shadowZones[i].size();
      }
      if (nExtrudeFaces != nShadowFaces) {
        FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
          << "Extruded faces " << nExtrudeFaces << endl
          << "is different from shadow faces. " << nShadowFaces
          << "This is not permitted " << endl
          << exit(FatalIOError);
      }
      extrudeMeshShadowFaces.setSize(nShadowFaces);
      zoneShadowFlipMap.setSize(nShadowFaces);
      zoneShadowID.setSize(nShadowFaces);
      nShadowFaces = 0;
      FOR_ALL(shadowZones, i) {
        const faceSet& fz = shadowZones[i];
        FOR_ALL_CONST_ITER(faceSet, fz, iter) {
          label faceI = iter.key();
          if (mesh.isInternalFace(faceI)) {
            FATAL_IO_ERROR_IN(args.executable().c_str(), dict)
              << "faceSet " << fz.name()
              << "contains internal faces."
              << " This is not permitted."
              << exit(FatalIOError);
          }
          extrudeMeshShadowFaces[nShadowFaces] = faceI;
          zoneShadowFlipMap[nShadowFaces] = false;
          zoneShadowID[nShadowFaces] = i;
          nShadowFaces++;
        }
      }
    }
  }
  const primitiveFacePatch extrudePatch{zoneFaces.xfer(), mesh.points()};
  Pstream::listCombineGather(isInternal, orEqOp<bool>());
  Pstream::listCombineScatter(isInternal);
  // Check zone either all internal or all external faces
  checkZoneInside(mesh, zoneNames, zoneID, extrudeMeshFaces, isInternal);
  const pointField& extrudePoints = extrudePatch.localPoints();
  const faceList& extrudeFaces = extrudePatch.localFaces();
  const labelListList& edgeFaces = extrudePatch.edgeFaces();
  Info << "extrudePatch :"
    << " faces:" << extrudePatch.size()
    << " points:" << extrudePatch.nPoints()
    << " edges:" << extrudePatch.nEdges()
    << nl
    << endl;
  // Determine per-extrude-edge info
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Corresponding mesh edges
  const labelList extrudeMeshEdges
  {
    extrudePatch.meshEdges(mesh.edges(), mesh.pointEdges())
  };
  const globalIndex globalExtrudeFaces{extrudePatch.size()};
  // Global pp faces per pp edge.
  labelListList extrudeEdgeGlobalFaces
  {
    globalEdgeFaces
    (
      mesh,
      globalExtrudeFaces,
      extrudePatch,
      extrudeMeshEdges
    )
  };
  List<Map<label>> compactMap;
  const mapDistribute extrudeEdgeFacesMap
  {
    globalExtrudeFaces,
    extrudeEdgeGlobalFaces,
    compactMap
  };
  // Determine min and max zone per edge
  labelList edgeMinZoneID;
  labelList edgeMaxZoneID;
  calcEdgeMinMaxZone
    (
      mesh,
      extrudePatch,
      extrudeMeshEdges,
      zoneID,
      extrudeEdgeFacesMap,
      extrudeEdgeGlobalFaces,
      edgeMinZoneID,
      edgeMaxZoneID
    );
  DynamicList<polyPatch*> regionPatches{patches.size()};
  // Copy all non-local patches since these are used on boundary edges of
  // the extrusion
  FOR_ALL(patches, patchI) {
    if (isA<processorPolyPatch>(patches[patchI]))
      continue;
    label newPatchI = regionPatches.size();
    regionPatches.append
      (
        patches[patchI].clone
        (
          patches,
          newPatchI,
          0,              // size
          0               // start
        ).ptr()
      );
  }
  // Add interface patches
  // ~~~~~~~~~~~~~~~~~~~~~
  // From zone to interface patch (region side)
  labelList interRegionTopPatch;
  labelList interRegionBottomPatch;
  addCouplingPatches
    (
      mesh,
      regionName,
      shellRegionName,
      zoneNames,
      zoneShadowNames,
      isInternal,
      meshZoneID,
      regionPatches,
      interRegionTopPatch,
      interRegionBottomPatch
    );
  // From zone to interface patch (mesh side)
  labelList interMeshTopPatch;
  labelList interMeshBottomPatch;
  if (adaptMesh) {
    // Add coupling patches to mesh
    // Clone existing patches
    DynamicList<polyPatch*> newPatches{patches.size()};
    FOR_ALL(patches, patchI) {
      newPatches.append(patches[patchI].clone(patches).ptr());
    }
    // Add new patches
    addCouplingPatches
      (
        mesh,
        regionName,
        shellRegionName,
        zoneNames,
        zoneShadowNames,
        isInternal,
        meshZoneID,
        newPatches,
        interMeshTopPatch,
        interMeshBottomPatch
      );
    // Add to mesh
    mesh.clearOut();
    mesh.removeFvBoundary();
    mesh.addFvPatches(newPatches, true);
    //!Note: from this point on mesh patches differs from regionPatches
  }
  // Patch per extruded face
  labelList extrudeTopPatchID{extrudePatch.size()};
  labelList extrudeBottomPatchID{extrudePatch.size()};
  FOR_ALL(zoneID, faceI) {
    extrudeTopPatchID[faceI] = interRegionTopPatch[zoneID[faceI]];
    extrudeBottomPatchID[faceI] = interRegionBottomPatch[zoneID[faceI]];
  }
  // Count how many patches on special edges of extrudePatch are necessary
  // - zoneXXX_sides
  // - zoneXXX_zoneYYY
  labelList zoneSidePatch{zoneNames.size(), 0};
  // Patch to use for minZone
  labelList zoneZonePatch_min{zoneNames.size()*zoneNames.size(), 0};
  // Patch to use for maxZone
  labelList zoneZonePatch_max{zoneNames.size()*zoneNames.size(), 0};
  countExtrudePatches
    (
      mesh,
      zoneNames.size(),
      extrudePatch,           // patch
      extrudeMeshFaces,       // mesh face per patch face
      extrudeMeshEdges,       // mesh edge per patch edge
      extrudeEdgeGlobalFaces, // global indexing per patch edge
      edgeMinZoneID,          // minZone per patch edge
      edgeMaxZoneID,          // maxZone per patch edge
      zoneSidePatch,          // per zone-side num edges that extrude into it
      zoneZonePatch_min       // per zone-zone num edges that extrude into it
    );
  // Now we'll have:
  //  zoneSidePatch[zoneA] : number of faces needed on the side of zoneA
  //  zoneZonePatch_min[zoneA,zoneB] : number of faces needed inbetween A,B
  // Add the zone-side patches.
  addZoneSidePatches
    (
      mesh,
      zoneNames,
      (oneD ? oneDPatchType : word::null),
      regionPatches,
      zoneSidePatch
    );
  // Add the patches inbetween zones
  addInterZonePatches
    (
      mesh,
      zoneNames,
      oneD,
      zoneZonePatch_min,
      zoneZonePatch_max,
      regionPatches
    );
  // Sets sidePatchID[edgeI] to interprocessor patch. Adds any
  // interprocessor or cyclic patches if necessary.
  labelList sidePatchID;
  addCoupledPatches
    (
      mesh,
      extrudePatch,
      extrudeMeshFaces,
      extrudeMeshEdges,
      extrudeEdgeFacesMap,
      extrudeEdgeGlobalFaces,
      sidePatchID,
      regionPatches
    );
  // Set patches to use for edges to be extruded into boundary faces
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // In order of edgeFaces: per edge, per originating face the
  // patch to use for the side face (from the extruded edge).
  // If empty size create an internal face.
  labelListList extrudeEdgePatches{extrudePatch.nEdges()};
  // Is edge a non-manifold edge
  PackedBoolList nonManifoldEdge{extrudePatch.nEdges()};
  // Note: logic has to be same as in countExtrudePatches.
  FOR_ALL(edgeFaces, edgeI) {
    const labelList& eFaces = edgeFaces[edgeI];
    labelList& ePatches = extrudeEdgePatches[edgeI];
    if (oneD) {
      ePatches.setSize(eFaces.size());
      FOR_ALL(eFaces, i) {
        ePatches[i] = zoneSidePatch[zoneID[eFaces[i]]];
      }
      if (oneDNonManifoldEdges) {
        //- Set nonManifoldEdge[edgeI] for non-manifold edges only
        //  The other option is to have non-manifold edges everywhere
        //  and generate space overlapping columns of cells.
        if (eFaces.size() != 2) {
          nonManifoldEdge[edgeI] = 1;
        }
      } else {
        nonManifoldEdge[edgeI] = 1;
      }
    } else if (eFaces.size() == 2) {
      label zone0 = zoneID[eFaces[0]];
      label zone1 = zoneID[eFaces[1]];
      if (zone0 != zone1) {  // || (cos(angle) > blabla))
        label minZone = min(zone0,zone1);
        label maxZone = max(zone0,zone1);
        label index = minZone*zoneNames.size()+maxZone;
        ePatches.setSize(eFaces.size());
        if (zone0 == minZone) {
          ePatches[0] = zoneZonePatch_min[index];
          ePatches[1] = zoneZonePatch_max[index];
        } else {
          ePatches[0] = zoneZonePatch_max[index];
          ePatches[1] = zoneZonePatch_min[index];
        }
        nonManifoldEdge[edgeI] = 1;
      }
    } else if (sidePatchID[edgeI] != -1) {
      // Coupled extrusion
      ePatches.setSize(eFaces.size());
      FOR_ALL(eFaces, i) {
        ePatches[i] = sidePatchID[edgeI];
      }
    } else {
      label faceI =
        findUncoveredPatchFace
        (
          mesh,
          UIndirectList<label>(extrudeMeshFaces, eFaces),
          extrudeMeshEdges[edgeI]
        );
      if (faceI != -1) {
        label newPatchI =
          findPatchID
          (
            regionPatches,
            patches[patches.whichPatch(faceI)].name()
          );
        ePatches.setSize(eFaces.size(), newPatchI);
      } else {
        ePatches.setSize(eFaces.size());
        FOR_ALL(eFaces, i) {
          ePatches[i] = zoneSidePatch[zoneID[eFaces[i]]];
        }
      }
      nonManifoldEdge[edgeI] = 1;
    }
  }
  // Assign point regions
  // ~~~~~~~~~~~~~~~~~~~~
  // Per face, per point the region number.
  faceList pointGlobalRegions;
  faceList pointLocalRegions;
  labelList localToGlobalRegion;
  createShellMesh::calcPointRegions
    (
      mesh.globalData(),
      extrudePatch,
      nonManifoldEdge,
      false,              // keep cyclic separated regions apart
      pointGlobalRegions,
      pointLocalRegions,
      localToGlobalRegion
    );
  // Per local region an originating point
  labelList localRegionPoints{localToGlobalRegion.size()};
  FOR_ALL(pointLocalRegions, faceI) {
    const face& f = extrudePatch.localFaces()[faceI];
    const face& pRegions = pointLocalRegions[faceI];
    FOR_ALL(pRegions, fp) {
      localRegionPoints[pRegions[fp]] = f[fp];
    }
  }
  // Calculate region normals by reducing local region normals
  pointField localRegionNormals{localToGlobalRegion.size()};

  {
    pointField localSum{localToGlobalRegion.size(), vector::zero};
    FOR_ALL(pointLocalRegions, faceI) {
      const face& pRegions = pointLocalRegions[faceI];
      FOR_ALL(pRegions, fp) {
        label localRegionI = pRegions[fp];
        localSum[localRegionI] += extrudePatch.faceNormals()[faceI];
      }
    }
    Map<point> globalSum{2*localToGlobalRegion.size()};
    FOR_ALL(localSum, localRegionI) {
      label globalRegionI = localToGlobalRegion[localRegionI];
      globalSum.insert(globalRegionI, localSum[localRegionI]);
    }
    // Reduce
    Pstream::mapCombineGather(globalSum, plusEqOp<point>());
    Pstream::mapCombineScatter(globalSum);
    FOR_ALL(localToGlobalRegion, localRegionI) {
      label globalRegionI = localToGlobalRegion[localRegionI];
      localRegionNormals[localRegionI] = globalSum[globalRegionI];
    }
    localRegionNormals /= mag(localRegionNormals);
  }
  // For debugging: dump hedgehog plot of normals
  if (false) {
    OFstream str{runTime.path()/"localRegionNormals.obj"};
    label vertI = 0;
    scalar thickness = model().sumThickness(1);
    FOR_ALL(pointLocalRegions, faceI) {
      const face& f = extrudeFaces[faceI];
      FOR_ALL(f, fp) {
        label region = pointLocalRegions[faceI][fp];
        const point& pt = extrudePoints[f[fp]];
        meshTools::writeOBJ(str, pt);
        vertI++;
        meshTools::writeOBJ
          (
            str,
            pt+thickness*localRegionNormals[region]
          );
        vertI++;
        str << "l " << vertI-1 << ' ' << vertI << nl;
      }
    }
  }
  // Use model to create displacements of first layer
  vectorField firstDisp{localRegionNormals.size()};
  FOR_ALL(firstDisp, regionI) {
    const point& regionPt =
      extrudePatch.points()
      [
        extrudePatch.meshPoints()[localRegionPoints[regionI]]
      ];
    const vector& n = localRegionNormals[regionI];
    firstDisp[regionI] = model()(regionPt, n, 1) - regionPt;
  }
  // Create a new mesh
  // ~~~~~~~~~~~~~~~~~
  createShellMesh extruder
  {
    extrudePatch,
    pointLocalRegions,
    localRegionPoints
  };
  autoPtr<mapPolyMesh> shellMap;
  fvMesh regionMesh
  {
    // IOobject
    {
      shellRegionName,
      meshInstance,
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE,
      false
    },
    xferCopy(pointField()),
    xferCopy(faceList()),
    xferCopy(labelList()),
    xferCopy(labelList()),
    false
  };
  // Add the new patches
  FOR_ALL(regionPatches, patchI) {
    polyPatch* ppPtr = regionPatches[patchI];
    regionPatches[patchI] = ppPtr->clone(regionMesh.boundaryMesh()).ptr();
    delete ppPtr;
  }
  regionMesh.clearOut();
  regionMesh.removeFvBoundary();
  regionMesh.addFvPatches(regionPatches, true);

  {
    polyTopoChange meshMod{regionPatches.size()};
    extruder.setRefinement
      (
        firstDisp,                              // first displacement
        model().expansionRatio(),
        model().nLayers(),                      // nLayers
        extrudeTopPatchID,
        extrudeBottomPatchID,
        extrudeEdgePatches,
        meshMod
      );
    // Enforce actual point posititions according to extrudeModel (model)
    // (extruder.setRefinement only does fixed expansionRatio)
    // The regionPoints and nLayers are looped in the same way as in
    // createShellMesh
    DynamicList<point>& newPoints =
      const_cast<DynamicList<point>&>(meshMod.points());
    label meshPointI = extrudePatch.localPoints().size();
    FOR_ALL(localRegionPoints, regionI) {
      label pointI = localRegionPoints[regionI];
      point pt = extrudePatch.localPoints()[pointI];
      const vector& n = localRegionNormals[regionI];
      for (label layerI = 1; layerI <= model().nLayers(); layerI++) {
        newPoints[meshPointI++] = model()(pt, n, layerI);
      }
    }
    shellMap =
      meshMod.changeMesh
      (
        regionMesh,     // mesh to change
        false           // inflate
      );
  }
  // Necessary?
  regionMesh.setInstance(meshInstance);
  // Update numbering on extruder.
  extruder.updateMesh(shellMap);
  // Calculate offsets from shell mesh back to original mesh
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  List<pointField> topOffsets{zoneNames.size()};
  List<pointField> bottomOffsets{zoneNames.size()};
  FOR_ALL(regionMesh.boundaryMesh(), patchI) {
    const polyPatch& pp = regionMesh.boundaryMesh()[patchI];
    if (!isA<mappedWallPolyPatch>(pp))
      continue;
    if (findIndex(interRegionTopPatch, patchI) != -1) {
      label zoneI = findIndex(interRegionTopPatch, patchI);
      topOffsets[zoneI] = calcOffset(extrudePatch, extruder, pp);
    } else if (findIndex(interRegionBottomPatch, patchI) != -1) {
      label zoneI = findIndex(interRegionBottomPatch, patchI);
      bottomOffsets[zoneI] = calcOffset(extrudePatch, extruder, pp);
    }
  }
  // Change top and bottom boundary conditions on regionMesh
  {
    // Correct top patches for offset
    setCouplingInfo
      (
        regionMesh,
        interRegionTopPatch,
        regionName,                 // name of main mesh
        sampleMode,                 // sampleMode
        topOffsets
      );
    // Correct bottom patches for offset
    setCouplingInfo
      (
        regionMesh,
        interRegionBottomPatch,
        regionName,
        sampleMode,                 // sampleMode
        bottomOffsets
      );
    // Remove any unused patches
    deleteEmptyPatches(regionMesh);
  }
  // Change top and bottom boundary conditions on main mesh
  if (adaptMesh) {
    // Correct top patches for offset
    setCouplingInfo
      (
        mesh,
        interMeshTopPatch,
        shellRegionName,                        // name of shell mesh
        sampleMode,                             // sampleMode
        -topOffsets
      );
    // Correct bottom patches for offset
    setCouplingInfo
      (
        mesh,
        interMeshBottomPatch,
        shellRegionName,
        sampleMode,
        -bottomOffsets
      );
  }
  // Write addressing from region mesh back to originating patch
  labelIOList cellToPatchFaceAddressing
  {
    // IOobject
    {
      "cellToPatchFaceAddressing",
      regionMesh.facesInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    extruder.cellToFaceMap()
  };
  cellToPatchFaceAddressing.note() = "cell to patch face addressing";
  labelIOList faceToPatchFaceAddressing
  {
    // IOobject
    {
      "faceToPatchFaceAddressing",
      regionMesh.facesInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    extruder.faceToFaceMap()
  };
  faceToPatchFaceAddressing.note() =
    "front/back face + turning index to patch face addressing";
  labelIOList faceToPatchEdgeAddressing
  {
    // IOobject
    {
      "faceToPatchEdgeAddressing",
      regionMesh.facesInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    extruder.faceToEdgeMap()
  };
  faceToPatchEdgeAddressing.note() =
    "side face to patch edge addressing";
  labelIOList pointToPatchPointAddressing
  {
    // IOobject
    {
      "pointToPatchPointAddressing",
      regionMesh.facesInstance(),
      regionMesh.meshSubDir,
      regionMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    extruder.pointToPointMap()
  };
  pointToPatchPointAddressing.note() =
    "point to patch point addressing";
  Info << "Writing mesh " << regionMesh.name()
    << " to " << regionMesh.facesInstance() << nl
    << endl;
  bool ok = regionMesh.write()
    && cellToPatchFaceAddressing.write()
    && faceToPatchFaceAddressing.write()
    && faceToPatchEdgeAddressing.write()
    && pointToPatchPointAddressing.write();
  if (!ok) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing mesh " << regionMesh.name()
      << " at location " << regionMesh.facesInstance()
      << exit(FatalError);
  }
  // See if we need to extrude coordinates as well
  {
    autoPtr<pointIOField> patchFaceCentresPtr;
    IOobject io
    {
      "patchFaceCentres",
      mesh.pointsInstance(),
      mesh.meshSubDir,
      mesh,
      IOobject::MUST_READ
    };
    if (io.headerOk()) {
      // Read patchFaceCentres and patchEdgeCentres
      Info << "Reading patch face,edge centres : "
        << io.name() << " and patchEdgeCentres" << endl;
      extrudeGeometricProperties
        (
          mesh,
          extrudePatch,
          extruder,
          regionMesh,
          model()
        );
    }
  }
  // Insert baffles into original mesh
  autoPtr<mapPolyMesh> addBafflesMap;
  if (adaptMesh) {
    polyTopoChange meshMod{mesh};
    // Modify faces to be in bottom (= always coupled) patch
    FOR_ALL(extrudeMeshFaces, zoneFaceI) {
      label meshFaceI = extrudeMeshFaces[zoneFaceI];
      label zoneI = zoneID[zoneFaceI];
      bool flip = zoneFlipMap[zoneFaceI];
      const face& f = mesh.faces()[meshFaceI];
      if (!flip) {
        meshMod.modifyFace
          (
            f,                          // modified face
            meshFaceI,                  // label of face being modified
            mesh.faceOwner()[meshFaceI],// owner
            -1,                         // neighbour
            false,                      // face flip
            interMeshBottomPatch[zoneI],// patch for face
            meshZoneID[zoneI],          // zone for face
            flip                        // face flip in zone
          );
      } else if (mesh.isInternalFace(meshFaceI)) {
        meshMod.modifyFace
          (
            f.reverseFace(),                // modified face
            meshFaceI,                      // label of modified face
            mesh.faceNeighbour()[meshFaceI],// owner
            -1,                             // neighbour
            true,                           // face flip
            interMeshBottomPatch[zoneI],    // patch for face
            meshZoneID[zoneI],              // zone for face
            !flip                           // face flip in zone
          );
      }
    }
    if (zoneShadowNames.size() > 0) {  //if there is a top faceZone specified
      FOR_ALL(extrudeMeshFaces, zoneFaceI) {
        label meshFaceI = extrudeMeshShadowFaces[zoneFaceI];
        label zoneI = zoneShadowID[zoneFaceI];
        bool flip = zoneShadowFlipMap[zoneFaceI];
        const face& f = mesh.faces()[meshFaceI];
        if (!flip) {
          meshMod.modifyFace
            (
              f,                          // modified face
              meshFaceI,                  // face being modified
              mesh.faceOwner()[meshFaceI],// owner
              -1,                         // neighbour
              false,                      // face flip
              interMeshTopPatch[zoneI],   // patch for face
              meshZoneID[zoneI],          // zone for face
              flip                        // face flip in zone
            );
        } else if (mesh.isInternalFace(meshFaceI)) {
          meshMod.modifyFace
            (
              f.reverseFace(),                // modified face
              meshFaceI,                      // label modified face
              mesh.faceNeighbour()[meshFaceI],// owner
              -1,                             // neighbour
              true,                           // face flip
              interMeshTopPatch[zoneI],       // patch for face
              meshZoneID[zoneI],              // zone for face
              !flip                           // face flip in zone
            );
        }
      }
    } else {
      // Add faces (using same points) to be in top patch
      FOR_ALL(extrudeMeshFaces, zoneFaceI) {
        label meshFaceI = extrudeMeshFaces[zoneFaceI];
        label zoneI = zoneID[zoneFaceI];
        bool flip = zoneFlipMap[zoneFaceI];
        const face& f = mesh.faces()[meshFaceI];
        if (!flip) {
          if (mesh.isInternalFace(meshFaceI)) {
            meshMod.addFace
              (
                f.reverseFace(),                // modified face
                mesh.faceNeighbour()[meshFaceI],// owner
                -1,                             // neighbour
                -1,                             // master point
                -1,                             // master edge
                meshFaceI,                      // master face
                true,                           // flip flux
                interMeshTopPatch[zoneI],       // patch for face
                -1,                             // zone for face
                false                           //face flip in zone
              );
          }
        } else {
          meshMod.addFace
            (
              f,                              // face
              mesh.faceOwner()[meshFaceI],    // owner
              -1,                             // neighbour
              -1,                             // master point
              -1,                             // master edge
              meshFaceI,                      // master face
              false,                          // flip flux
              interMeshTopPatch[zoneI],       // patch for face
              -1,                             // zone for face
              false                           // zone flip
            );
        }
      }
    }
    // Change the mesh. Change points directly (no inflation).
    addBafflesMap = meshMod.changeMesh(mesh, false);
    // Update fields
    mesh.updateMesh(addBafflesMap);
    // Update maps! e.g. faceToPatchFaceAddressing
    // Move mesh (since morphing might not do this)
    if (addBafflesMap().hasMotionPoints()) {
      mesh.movePoints(addBafflesMap().preMotionPoints());
    }
    mesh.setInstance(meshInstance);
    // Remove any unused patches
    deleteEmptyPatches(mesh);
    Info << "Writing mesh " << mesh.name()
      << " to " << mesh.facesInstance() << nl
      << endl;
    if (!mesh.write()) {
      FATAL_ERROR_IN(args.executable())
        << "Failed writing mesh " << mesh.name()
        << " at location " << mesh.facesInstance()
        << exit(FatalError);
    }
  }
  Info << "End\n" << endl;
  return 0;
}

