// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_poly_patch.hpp"
#include "sync_tools.hpp"
#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "sortable_list.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "face_set.hpp"
#include "ioptr_list.hpp"
#include "poly_topo_change.hpp"
#include "poly_modify_face.hpp"
#include "word_re_list.hpp"


using namespace mousse;


namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(IOPtrList<dictionary>, 0);

}


void changePatchID
(
  const polyMesh& mesh,
  const label faceID,
  const label patchID,
  polyTopoChange& meshMod
)
{
  const label zoneID = mesh.faceZones().whichZone(faceID);
  bool zoneFlip = false;
  if (zoneID >= 0) {
    const faceZone& fZone = mesh.faceZones()[zoneID];
    zoneFlip = fZone.flipMap()[fZone.whichFace(faceID)];
  }
  meshMod.setAction
  (
    polyModifyFace
    {
      mesh.faces()[faceID],               // face
      faceID,                             // face ID
      mesh.faceOwner()[faceID],           // owner
      -1,                                 // neighbour
      false,                              // flip flux
      patchID,                            // patch ID
      false,                              // remove from zone
      zoneID,                             // zone ID
      zoneFlip                            // zone flip
    }
  );
}


// Filter out the empty patches.
void filterPatches(polyMesh& mesh, const HashSet<word>& addedPatchNames)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Patches to keep
  DynamicList<polyPatch*> allPatches{patches.size()};
  label nOldPatches = returnReduce(patches.size(), sumOp<label>());
  // Copy old patches.
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    // Note: reduce possible since non-proc patches guaranteed in same order
    if (isA<processorPolyPatch>(pp))
      continue;
    // Add if
    // - non zero size
    // - or added from the createPatchDict
    // - or cyclic (since referred to by other cyclic half or
    //   proccyclic)
    if (addedPatchNames.found(pp.name())
        || returnReduce(pp.size(), sumOp<label>()) > 0
        || isA<coupledPolyPatch>(pp)) {
      allPatches.append
      (
        pp.clone
        (
          patches,
          allPatches.size(),
          pp.size(),
          pp.start()
        ).ptr()
      );
    } else {
      Info << "Removing zero-sized patch " << pp.name()
        << " type " << pp.type()
        << " at position " << patchI << endl;
    }
  }
  // Copy non-empty processor patches
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    if (!isA<processorPolyPatch>(pp))
      continue;
    if (pp.size()) {
      allPatches.append
      (
        pp.clone
        (
          patches,
          allPatches.size(),
          pp.size(),
          pp.start()
        ).ptr()
      );
    } else {
      Info << "Removing empty processor patch " << pp.name()
        << " at position " << patchI << endl;
    }
  }
  label nAllPatches = returnReduce(allPatches.size(), sumOp<label>());
  if (nAllPatches != nOldPatches) {
    Info << "Removing patches." << endl;
    allPatches.shrink();
    mesh.removeBoundary();
    mesh.addPatches(allPatches);
  } else {
    Info << "No patches removed." << endl;
    FOR_ALL(allPatches, i) {
      delete allPatches[i];
    }
  }
}


// Dump for all patches the current match
void dumpCyclicMatch(const fileName& prefix, const polyMesh& mesh)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI) {
    if (!isA<cyclicPolyPatch>(patches[patchI])
        || !refCast<const cyclicPolyPatch>(patches[patchI]).owner())
      continue;
    const cyclicPolyPatch& cycPatch =
      refCast<const cyclicPolyPatch>(patches[patchI]);
    // Dump patches
    {
      OFstream str{prefix+cycPatch.name()+".obj"};
      Pout << "Dumping " << cycPatch.name()
        << " faces to " << str.name() << endl;
      meshTools::writeOBJ
      (
        str,
        cycPatch,
        cycPatch.points()
      );
    }

    const cyclicPolyPatch& nbrPatch = cycPatch.neighbPatch();

    {
      OFstream str{prefix+nbrPatch.name()+".obj"};
      Pout << "Dumping " << nbrPatch.name()
        << " faces to " << str.name() << endl;
      meshTools::writeOBJ
      (
        str,
        nbrPatch,
        nbrPatch.points()
      );
    }
    // Lines between corresponding face centres
    OFstream str{prefix+cycPatch.name()+nbrPatch.name()+"_match.obj"};
    label vertI = 0;
    Pout << "Dumping cyclic match as lines between face centres to "
      << str.name() << endl;
    FOR_ALL(cycPatch, faceI) {
      const point& fc0 = mesh.faceCentres()[cycPatch.start()+faceI];
      meshTools::writeOBJ(str, fc0);
      vertI++;
      const point& fc1 = mesh.faceCentres()[nbrPatch.start()+faceI];
      meshTools::writeOBJ(str, fc1);
      vertI++;
      str<< "l " << vertI-1 << ' ' << vertI << nl;
    }
  }
}


void separateList
(
  const vectorField& separation,
  UList<vector>& field
)
{
  if (separation.size() == 1) {
    // Single value for all.
    FOR_ALL(field, i) {
      field[i] += separation[0];
    }
  } else if (separation.size() == field.size()) {
    FOR_ALL(field, i) {
      field[i] += separation[i];
    }
  } else {
    FATAL_ERROR_IN
    (
      "separateList(const vectorField&, UList<vector>&)"
    )
    << "Sizes of field and transformation not equal. field:"
    << field.size() << " transformation:" << separation.size()
    << abort(FatalError);
  }
}


// Synchronise points on both sides of coupled boundaries.
template<class CombineOp>
void syncPoints
(
  const polyMesh& mesh,
  pointField& points,
  const CombineOp& cop,
  const point& nullValue
)
{
  if (points.size() != mesh.nPoints()) {
    FATAL_ERROR_IN
    (
      "syncPoints"
      "(const polyMesh&, pointField&, const CombineOp&, const point&)"
    )
    << "Number of values " << points.size()
    << " is not equal to the number of points in the mesh "
    << mesh.nPoints() << abort(FatalError);
  }
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Is there any coupled patch with transformation?
  bool hasTransformation = false;
  if (Pstream::parRun()) {
    // Send
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (!isA<processorPolyPatch>(pp) || pp.nPoints() <= 0
          || !refCast<const processorPolyPatch>(pp).owner())
        continue;
      const processorPolyPatch& procPatch =
        refCast<const processorPolyPatch>(pp);
      // Get data per patchPoint in neighbouring point numbers.
      pointField patchInfo{procPatch.nPoints(), nullValue};
      const labelList& meshPts = procPatch.meshPoints();
      const labelList& nbrPts = procPatch.neighbPoints();
      FOR_ALL(nbrPts, pointI) {
        label nbrPointI = nbrPts[pointI];
        if (nbrPointI >= 0 && nbrPointI < patchInfo.size()) {
          patchInfo[nbrPointI] = points[meshPts[pointI]];
        }
      }
      OPstream toNbr{Pstream::blocking, procPatch.neighbProcNo()};
      toNbr << patchInfo;
    }
    // Receive and set.
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (!isA<processorPolyPatch>(pp) || pp.nPoints() <= 0
          || refCast<const processorPolyPatch>(pp).owner())
        continue;
      const processorPolyPatch& procPatch =
        refCast<const processorPolyPatch>(pp);
      pointField nbrPatchInfo{procPatch.nPoints()};

      {
        // We do not know the number of points on the other side
        // so cannot use Pstream::read.
        IPstream fromNbr
        {
          Pstream::blocking,
          procPatch.neighbProcNo()
        };
        fromNbr >> nbrPatchInfo;
      }
      // Null any value which is not on neighbouring processor
      nbrPatchInfo.setSize(procPatch.nPoints(), nullValue);
      if (!procPatch.parallel()) {
        hasTransformation = true;
        transformList(procPatch.forwardT(), nbrPatchInfo);
      } else if (procPatch.separated()) {
        hasTransformation = true;
        separateList(-procPatch.separation(), nbrPatchInfo);
      }
      const labelList& meshPts = procPatch.meshPoints();
      FOR_ALL(meshPts, pointI) {
        label meshPointI = meshPts[pointI];
        points[meshPointI] = nbrPatchInfo[pointI];
      }
    }
  }
  // Do the cyclics.
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    if (!isA<cyclicPolyPatch>(pp)
        || !refCast<const cyclicPolyPatch>(pp).owner())
      continue;
    const cyclicPolyPatch& cycPatch = refCast<const cyclicPolyPatch>(pp);
    const edgeList& coupledPoints = cycPatch.coupledPoints();
    const labelList& meshPts = cycPatch.meshPoints();
    const cyclicPolyPatch& nbrPatch = cycPatch.neighbPatch();
    const labelList& nbrMeshPts = nbrPatch.meshPoints();
    pointField half0Values{coupledPoints.size()};
    FOR_ALL(coupledPoints, i) {
      const edge& e = coupledPoints[i];
      label point0 = meshPts[e[0]];
      half0Values[i] = points[point0];
    }
    if (!cycPatch.parallel()) {
      hasTransformation = true;
      transformList(cycPatch.reverseT(), half0Values);
    } else if (cycPatch.separated()) {
      hasTransformation = true;
      separateList(cycPatch.separation(), half0Values);
    }
    FOR_ALL(coupledPoints, i) {
      const edge& e = coupledPoints[i];
      label point1 = nbrMeshPts[e[1]];
      points[point1] = half0Values[i];
    }
  }
  //- Note: hasTransformation is only used for warning messages so
  //  reduction not strictly nessecary.
  //reduce(hasTransformation, orOp<bool>());
  // Synchronize multiple shared points.
  const globalMeshData& pd = mesh.globalData();
  if (pd.nGlobalPoints() > 0) {
    if (hasTransformation) {
      WARNING_IN
      (
        "syncPoints"
        "(const polyMesh&, pointField&, const CombineOp&, const point&)"
      )
      << "There are decomposed cyclics in this mesh with"
      << " transformations." << endl
      << "This is not supported. The result will be incorrect"
      << endl;
    }
    // Values on shared points.
    pointField sharedPts{pd.nGlobalPoints(), nullValue};
    FOR_ALL(pd.sharedPointLabels(), i) {
      label meshPointI = pd.sharedPointLabels()[i];
      // Fill my entries in the shared points
      sharedPts[pd.sharedPointAddr()[i]] = points[meshPointI];
    }
    // Combine on master.
    Pstream::listCombineGather(sharedPts, cop);
    Pstream::listCombineScatter(sharedPts);
    // Now we will all have the same information. Merge it back with
    // my local information.
    FOR_ALL(pd.sharedPointLabels(), i) {
      label meshPointI = pd.sharedPointLabels()[i];
      points[meshPointI] = sharedPts[pd.sharedPointAddr()[i]];
    }
  }
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  #include "add_region_option.inc"
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  mousse::word meshRegionName = polyMesh::defaultRegion;
  args.optionReadIfPresent("region", meshRegionName);
  const bool overwrite = args.optionFound("overwrite");
  #include "create_named_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const word dictName("createPatchDict");
  #include "set_system_mesh_dictionary_io.inc"
  Info << "Reading " << dictName << nl << endl;
  IOdictionary dict{dictIO};
  // Whether to synchronise points
  const Switch pointSync{dict.lookup("pointSync")};
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // If running parallel check same patches everywhere
  patches.checkParallelSync(true);
  dumpCyclicMatch("initial_", mesh);
  // Read patch construct info from dictionary
  PtrList<dictionary> patchSources{dict.lookup("patches")};
  HashSet<word> addedPatchNames;
  FOR_ALL(patchSources, addedI) {
    const dictionary& dict = patchSources[addedI];
    addedPatchNames.insert(dict.lookup("name"));
  }
  // 1. Add all new patches
  // ~~~~~~~~~~~~~~~~~~~~~~
  if (patchSources.size()) {
    // Old and new patches.
    DynamicList<polyPatch*> allPatches{patches.size() + patchSources.size()};
    label startFaceI = mesh.nInternalFaces();
    // Copy old patches.
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (isA<processorPolyPatch>(pp))
        continue;
      allPatches.append
      (
        pp.clone
        (
          patches,
          patchI,
          pp.size(),
          startFaceI
        ).ptr()
      );
      startFaceI += pp.size();
    }
    FOR_ALL(patchSources, addedI) {
      const dictionary& dict = patchSources[addedI];
      word patchName(dict.lookup("name"));
      label destPatchI = patches.findPatchID(patchName);
      if (destPatchI == -1) {
        dictionary patchDict{dict.subDict("patchInfo")};
        destPatchI = allPatches.size();
        Info << "Adding new patch " << patchName
          << " as patch " << destPatchI
          << " from " << patchDict << endl;
        patchDict.set("nFaces", 0);
        patchDict.set("startFace", startFaceI);
        // Add an empty patch.
        allPatches.append
        (
          polyPatch::New
          (
            patchName,
            patchDict,
            destPatchI,
            patches
          ).ptr()
        );
      } else {
        Info << "Patch '" << patchName << "' already exists.  Only "
          << "moving patch faces - type will remain the same" << endl;
      }
    }
    // Copy old patches.
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (!isA<processorPolyPatch>(pp))
        continue;
      allPatches.append
      (
        pp.clone
        (
          patches,
          patchI,
          pp.size(),
          startFaceI
        ).ptr()
      );
      startFaceI += pp.size();
    }
    allPatches.shrink();
    mesh.removeBoundary();
    mesh.addPatches(allPatches);
    Info << endl;
  }
  // 2. Repatch faces
  // ~~~~~~~~~~~~~~~~
  polyTopoChange meshMod{mesh};
  FOR_ALL(patchSources, addedI) {
    const dictionary& dict = patchSources[addedI];
    const word patchName{dict.lookup("name")};
    label destPatchI = patches.findPatchID(patchName);
    if (destPatchI == -1) {
      FATAL_ERROR_IN(args.executable())
        << "patch " << patchName << " not added. Problem."
        << abort(FatalError);
    }
    const word sourceType{dict.lookup("constructFrom")};
    if (sourceType == "patches") {
      labelHashSet patchSources
      {
        patches.patchSet
        (
          wordReList{dict.lookup("patches")}
        )
      };
      // Repatch faces of the patches.
      FOR_ALL_CONST_ITER(labelHashSet, patchSources, iter) {
        const polyPatch& pp = patches[iter.key()];
        Info << "Moving faces from patch " << pp.name()
          << " to patch " << destPatchI << endl;
        FOR_ALL(pp, i) {
          changePatchID
          (
            mesh,
            pp.start() + i,
            destPatchI,
            meshMod
          );
        }
      }
    } else if (sourceType == "set") {
      const word setName{dict.lookup("set")};
      faceSet faces{mesh, setName};
      Info << "Read " << returnReduce(faces.size(), sumOp<label>())
        << " faces from faceSet " << faces.name() << endl;
      // Sort (since faceSet contains faces in arbitrary order)
      labelList faceLabels{faces.toc()};
      SortableList<label> patchFaces{faceLabels};
      FOR_ALL(patchFaces, i) {
        label faceI = patchFaces[i];
        if (mesh.isInternalFace(faceI)) {
          FATAL_ERROR_IN(args.executable())
            << "Face " << faceI << " specified in set "
            << faces.name()
            << " is not an external face of the mesh." << endl
            << "This application can only repatch existing boundary"
            << " faces." << exit(FatalError);
        }
        changePatchID
        (
          mesh,
          faceI,
          destPatchI,
          meshMod
        );
      }
    } else {
      FATAL_ERROR_IN(args.executable())
        << "Invalid source type " << sourceType << endl
        << "Valid source types are 'patches' 'set'" << exit(FatalError);
    }
  }
  Info << endl;
  // Change mesh, use inflation to reforce calculation of transformation
  // tensors.
  Info << "Doing topology modification to order faces." << nl << endl;
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, true);
  mesh.movePoints(map().preMotionPoints());
  dumpCyclicMatch("coupled_", mesh);
  // Synchronise points.
  if (!pointSync) {
    Info << "Not synchronising points." << nl << endl;
  } else {
    Info << "Synchronising points." << nl << endl;
    // This is a bit tricky. Both normal and position might be out and
    // current separation also includes the normal
    // ( separation_ = (nf&(Cr - Cf))*nf ).
    // For cyclic patches:
    // - for separated ones use user specified offset vector
    FOR_ALL(mesh.boundaryMesh(), patchI) {
      const polyPatch& pp = mesh.boundaryMesh()[patchI];
      if (!pp.size() || !isA<coupledPolyPatch>(pp))
        continue;
      const coupledPolyPatch& cpp = refCast<const coupledPolyPatch>(pp);
      if (cpp.separated()) {
        Info << "On coupled patch " << pp.name()
          << " separation[0] was "
          << cpp.separation()[0] << endl;
        if (isA<cyclicPolyPatch>(pp) && pp.size()) {
          const cyclicPolyPatch& cycpp =
            refCast<const cyclicPolyPatch>(pp);
          if (cycpp.transform() == cyclicPolyPatch::TRANSLATIONAL) {
            // Force to wanted separation
            Info << "On cyclic translation patch " << pp.name()
              << " forcing uniform separation of "
              << cycpp.separationVector() << endl;
            const_cast<vectorField&>(cpp.separation()) =
              pointField{1, cycpp.separationVector()};
          } else {
            const cyclicPolyPatch& nbr = cycpp.neighbPatch();
            const_cast<vectorField&>(cpp.separation()) =
              pointField
              {
                1,
                nbr[0].centre(mesh.points()) - cycpp[0].centre(mesh.points())
              };
          }
        }
        Info << "On coupled patch " << pp.name()
          << " forcing uniform separation of "
          << cpp.separation() << endl;
      } else if (!cpp.parallel()) {
        Info << "On coupled patch " << pp.name()
          << " forcing uniform rotation of "
          << cpp.forwardT()[0] << endl;
        const_cast<tensorField&>(cpp.forwardT()).setSize(1);
        const_cast<tensorField&>(cpp.reverseT()).setSize(1);
        Info << "On coupled patch " << pp.name()
          << " forcing uniform rotation of "
          << cpp.forwardT() << endl;
      }
    }
    Info << "Synchronising points." << endl;
    pointField newPoints{mesh.points()};
    syncPoints
    (
      mesh,
      newPoints,
      minMagSqrEqOp<vector>(),
      point(GREAT, GREAT, GREAT)
    );
    scalarField diff{mag(newPoints-mesh.points())};
    Info << "Points changed by average:" << gAverage(diff)
      << " max:" << gMax(diff) << nl << endl;
    mesh.movePoints(newPoints);
  }
  // 3. Remove zeros-sized patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Info << "Removing patches with no faces in them." << nl<< endl;
  filterPatches(mesh, addedPatchNames);
  dumpCyclicMatch("final_", mesh);
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  if (!overwrite) {
    runTime++;
  } else {
    mesh.setInstance(oldInstance);
  }
  // Write resulting mesh
  Info << "Writing repatched mesh to " << runTime.timeName() << nl << endl;
  mesh.write();
  Info << "End\n" << endl;
  return 0;
}

