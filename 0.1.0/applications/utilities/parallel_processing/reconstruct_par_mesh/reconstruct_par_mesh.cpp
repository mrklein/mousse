// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "ioobject_list.hpp"
#include "label_io_list.hpp"
#include "processor_poly_patch.hpp"
#include "map_added_poly_mesh.hpp"
#include "poly_mesh_adder.hpp"
#include "face_couple_info.hpp"
#include "fv_mesh_adder.hpp"
#include "poly_topo_change.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


using namespace mousse;


// Tolerance (as fraction of the bounding box). Needs to be fairly lax since
// usually meshes get written with limited precision (6 digits)
static const scalar defaultMergeTol = 1e-7;


static void renumber
(
  const labelList& map,
  labelList& elems
)
{
  FOR_ALL(elems, i) {
    if (elems[i] >= 0) {
      elems[i] = map[elems[i]];
    }
  }
}


// Determine which faces are coupled. Uses geometric merge distance.
// Looks either at all boundaryFaces (fullMatch) or only at the
// procBoundaries for procI. Assumes that masterMesh contains already merged
// all the processors < procI.
autoPtr<faceCoupleInfo> determineCoupledFaces
(
  const bool fullMatch,
  const label procI,
  const polyMesh& masterMesh,
  const polyMesh& meshToAdd,
  const scalar mergeDist
)
{
  if (fullMatch || masterMesh.nCells() == 0) {
    return
      autoPtr<faceCoupleInfo>
      {
        new faceCoupleInfo
        {
          masterMesh,
          meshToAdd,
          mergeDist,      // Absolute merging distance
          true            // Matching faces identical
        }
      };
  } else {
    // Pick up all patches on masterMesh ending in "toDDD" where DDD is
    // the processor number procI.
    const polyBoundaryMesh& masterPatches = masterMesh.boundaryMesh();
    const string toProcString{"to" + name(procI)};
    DynamicList<label> masterFaces
    {
      masterMesh.nFaces() - masterMesh.nInternalFaces()
    };
    FOR_ALL(masterPatches, patchI) {
      const polyPatch& pp = masterPatches[patchI];
      if (isA<processorPolyPatch>(pp)
          && (pp.name().rfind(toProcString)
              == (pp.name().size() - toProcString.size()))) {
        label meshFaceI = pp.start();
        FOR_ALL(pp, i) {
          masterFaces.append(meshFaceI++);
        }
      }
    }
    masterFaces.shrink();
    // Pick up all patches on meshToAdd ending in "procBoundaryDDDtoYYY"
    // where DDD is the processor number procI and YYY is < procI.
    const polyBoundaryMesh& addPatches = meshToAdd.boundaryMesh();
    DynamicList<label> addFaces
    {
      meshToAdd.nFaces() - meshToAdd.nInternalFaces()
    };
    FOR_ALL(addPatches, patchI) {
      const polyPatch& pp = addPatches[patchI];
      if (!isA<processorPolyPatch>(pp))
        continue;
      bool isConnected = false;
      for (label mergedProcI = 0; mergedProcI < procI; mergedProcI++) {
        const string fromProcString
        {
          "procBoundary" + name(procI) + "to" + name(mergedProcI)
        };
        if (pp.name() == fromProcString) {
          isConnected = true;
          break;
        }
      }
      if (isConnected) {
        label meshFaceI = pp.start();
        FOR_ALL(pp, i) {
          addFaces.append(meshFaceI++);
        }
      }
    }
    addFaces.shrink();
    return
      autoPtr<faceCoupleInfo>
      {
        new faceCoupleInfo
        {
          masterMesh,
          masterFaces,
          meshToAdd,
          addFaces,
          mergeDist,      // Absolute merging distance
          true,           // Matching faces identical?
          false,          // If perfect match are faces already ordered
                          // (e.g. processor patches)
          false           // are faces each on separate patch?
        }
      };
  }
}


autoPtr<mapPolyMesh> mergeSharedPoints
(
  const scalar mergeDist,
  polyMesh& mesh,
  labelListList& pointProcAddressing
)
{
  // Find out which sets of points get merged and create a map from
  // mesh point to unique point.
  Map<label> pointToMaster
  {
    fvMeshAdder::findSharedPoints(mesh, mergeDist)
  };
  Info << "mergeSharedPoints : detected " << pointToMaster.size()
    << " points that are to be merged." << endl;
  if (returnReduce(pointToMaster.size(), sumOp<label>()) == 0) {
    return autoPtr<mapPolyMesh>{nullptr};
  }
  polyTopoChange meshMod{mesh};
  fvMeshAdder::mergePoints(mesh, pointToMaster, meshMod);
  // Change the mesh (no inflation). Note: parallel comms allowed.
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false, true);
  // Update fields. No inflation, parallel sync.
  mesh.updateMesh(map);
  // pointProcAddressing give indices into the master mesh so adapt them
  // for changed point numbering.
  // Adapt constructMaps for merged points.
  FOR_ALL(pointProcAddressing, procI) {
    labelList& constructMap = pointProcAddressing[procI];
    FOR_ALL(constructMap, i) {
      label oldPointI = constructMap[i];
      // New label of point after changeMesh.
      label newPointI = map().reversePointMap()[oldPointI];
      if (newPointI < -1) {
        constructMap[i] = -newPointI - 2;
      } else if (newPointI >= 0) {
        constructMap[i] = newPointI;
      } else {
        FATAL_ERROR_IN("fvMeshDistribute::mergeSharedPoints()")
          << "Problem. oldPointI:" << oldPointI
          << " newPointI:" << newPointI << abort(FatalError);
      }
    }
  }
  return map;
}


boundBox procBounds
(
  const argList& args,
  const PtrList<Time>& databases,
  const word& regionDir
)
{
  boundBox bb = boundBox::invertedBox;
  FOR_ALL(databases, procI) {
    fileName pointsInstance
    {
      databases[procI].findInstance(regionDir/polyMesh::meshSubDir, "points")
    };
    if (pointsInstance != databases[procI].timeName()) {
      FATAL_ERROR_IN(args.executable())
        << "Your time was specified as " << databases[procI].timeName()
        << " but there is no polyMesh/points in that time." << endl
        << "(there is a points file in " << pointsInstance
        << ")" << endl
        << "Please rerun with the correct time specified"
        << " (through the -constant, -time or -latestTime "
        << "(at your option)."
        << endl << exit(FatalError);
    }
    Info << "Reading points from "
      << databases[procI].caseName()
      << " for time = " << databases[procI].timeName()
      << nl << endl;
    pointIOField points
    {
      {
        "points",
        databases[procI].findInstance(regionDir/polyMesh::meshSubDir, "points"),
        regionDir/polyMesh::meshSubDir,
        databases[procI],
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      }
    };
    boundBox domainBb(points, false);
    bb.min() = min(bb.min(), domainBb.min());
    bb.max() = max(bb.max(), domainBb.max());
  }
  return bb;
}


void writeCellDistance
(
  Time& runTime,
  const fvMesh& masterMesh,
  const labelListList& cellProcAddressing
)
{
  // Write the decomposition as labelList for use with 'manual'
  // decomposition method.
  labelIOList cellDecomposition
  {
    {
      "cellDecomposition",
      masterMesh.facesInstance(),
      masterMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    masterMesh.nCells()
  };
  FOR_ALL(cellProcAddressing, procI) {
    const labelList& pCells = cellProcAddressing[procI];
    UIndirectList<label>{cellDecomposition, pCells} = procI;
  }
  cellDecomposition.write();
  Info << nl << "Wrote decomposition to "
    << cellDecomposition.objectPath()
    << " for use in manual decomposition." << endl;
  // Write as volScalarField for postprocessing. Change time to 0
  // if was 'constant'
  {
    const scalar oldTime = runTime.value();
    const label oldIndex = runTime.timeIndex();
    if (runTime.timeName() == runTime.constant() && oldIndex == 0) {
      runTime.setTime(0, oldIndex+1);
    }
    volScalarField cellDist
    {
      {
        "cellDist",
        runTime.timeName(),
        masterMesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      masterMesh,
      {"cellDist", dimless, 0},
      zeroGradientFvPatchScalarField::typeName
    };
    FOR_ALL(cellDecomposition, cellI) {
      cellDist[cellI] = cellDecomposition[cellI];
    }
    cellDist.write();
    Info << nl << "Wrote decomposition as volScalarField to "
      << cellDist.name() << " for use in postprocessing."
      << endl;
    // Restore time
    runTime.setTime(oldTime, oldIndex);
  }
}


int main(int argc, char *argv[])
{
  argList::addNote("reconstruct a mesh using geometric information only");
  // Enable -constant ... if someone really wants it
  // Enable -withZero to prevent accidentally trashing the initial fields
  timeSelector::addOptions(true, true);
  argList::noParallel();
  argList::addOption
  (
    "mergeTol",
    "scalar",
    "specify the merge distance relative to the bounding box size "
    "(default 1e-7)"
  );
  argList::addBoolOption
  (
    "fullMatch",
    "do (slower) geometric matching on all boundary faces"
  );
  argList::addBoolOption
  (
    "cellDist",
    "write cell distribution as a labelList - for use with 'manual' "
    "decomposition method or as a volScalarField for post-processing."
  );
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  Info << "This is an experimental tool which tries to merge"
    << " individual processor" << nl
    << "meshes back into one master mesh. Use it if the original"
    << " master mesh has" << nl
    << "been deleted or if the processor meshes have been modified"
    << " (topology change)." << nl
    << "This tool will write the resulting mesh to a new time step"
    << " and construct" << nl
    << "xxxxProcAddressing files in the processor meshes so"
    << " reconstructPar can be" << nl
    << "used to regenerate the fields on the master mesh." << nl
    << nl
    << "Not well tested & use at your own risk!" << nl
    << endl;
  word regionName = polyMesh::defaultRegion;
  word regionDir = word::null;
  if (args.optionReadIfPresent("region", regionName)
      && regionName != polyMesh::defaultRegion) {
    regionDir = regionName;
    Info << "Operating on region " << regionName << nl << endl;
  }
  scalar mergeTol = defaultMergeTol;
  args.optionReadIfPresent("mergeTol", mergeTol);
  scalar writeTol = mousse::pow(10.0, -scalar(IOstream::defaultPrecision()));
  Info << "Merge tolerance : " << mergeTol << nl
    << "Write tolerance : " << writeTol << endl;
  if (runTime.writeFormat() == IOstream::ASCII && mergeTol < writeTol) {
    FATAL_ERROR_IN(args.executable())
      << "Your current settings specify ASCII writing with "
      << IOstream::defaultPrecision() << " digits precision." << endl
      << "Your merging tolerance (" << mergeTol << ") is finer than this."
      << endl
      << "Please change your writeFormat to binary"
      << " or increase the writePrecision" << endl
      << "or adjust the merge tolerance (-mergeTol)."
      << exit(FatalError);
  }
  const bool fullMatch = args.optionFound("fullMatch");
  if (fullMatch) {
    Info << "Doing geometric matching on all boundary faces." << nl << endl;
  } else {
    Info << "Doing geometric matching on correct procBoundaries only."
      << nl << "This assumes a correct decomposition." << endl;
  }
  bool writeCellDist = args.optionFound("cellDist");
  int nProcs = 0;
  while (isDir(args.path()/fileName(word("processor") + name(nProcs)))) {
    nProcs++;
  }
  Info << "Found " << nProcs << " processor directories" << nl << endl;
  // Read all time databases
  PtrList<Time> databases{nProcs};
  FOR_ALL(databases, procI) {
    Info << "Reading database "
      << args.caseName()/fileName(word("processor") + name(procI))
      << endl;
    databases.set
    (
      procI,
      new Time
      {
        Time::controlDictName,
        args.rootPath(),
        args.caseName()/fileName(word("processor") + name(procI))
      }
    );
  }
  // Use the times list from the master processor
  // and select a subset based on the command-line options
  instantList timeDirs = timeSelector::select(databases[0].times(), args);
  // Loop over all times
  FOR_ALL(timeDirs, timeI) {
    // Set time for global database
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << nl << endl;
    // Set time for all databases
    FOR_ALL(databases, procI) {
      databases[procI].setTime(timeDirs[timeI], timeI);
    }
    const auto& d0 = databases[0];
    const fileName meshPath =
      d0.path()/d0.timeName()/regionDir/polyMesh::meshSubDir;
    if (!isFile(meshPath/"faces")) {
      Info << "No mesh." << nl << endl;
      continue;
    }
    // Read point on individual processors to determine merge tolerance
    // (otherwise single cell domains might give problems)
    const boundBox bb = procBounds(args, databases, regionDir);
    const scalar mergeDist = mergeTol*bb.mag();
    Info
      << "Overall mesh bounding box  : " << bb << nl
      << "Relative tolerance         : " << mergeTol << nl
      << "Absolute matching distance : " << mergeDist << nl
      << endl;
    // Addressing from processor to reconstructed case
    labelListList cellProcAddressing{nProcs};
    labelListList faceProcAddressing{nProcs};
    labelListList pointProcAddressing{nProcs};
    labelListList boundaryProcAddressing{nProcs};
    // Internal faces on the final reconstructed mesh
    label masterInternalFaces;
    // Owner addressing on the final reconstructed mesh
    labelList masterOwner;

    {
      // Construct empty mesh.
      Info << "Constructing empty mesh to add to." << nl << endl;
      fvMesh masterMesh
      {
        {
          regionName,
          runTime.timeName(),
          runTime,
          IOobject::NO_READ
        },
        xferCopy(pointField()),
        xferCopy(faceList()),
        xferCopy(cellList())
      };
      for (label procI = 0; procI < nProcs; procI++) {
        Info << "Reading mesh to add from "
          << databases[procI].caseName()
          << " for time = " << databases[procI].timeName()
          << nl << endl;
        fvMesh meshToAdd
        {
          {
            regionName,
            databases[procI].timeName(),
            databases[procI]
          }
        };
        // Initialize its addressing
        cellProcAddressing[procI] = identity(meshToAdd.nCells());
        faceProcAddressing[procI] = identity(meshToAdd.nFaces());
        pointProcAddressing[procI] = identity(meshToAdd.nPoints());
        boundaryProcAddressing[procI] =
          identity(meshToAdd.boundaryMesh().size());
        // Find geometrically shared points/faces.
        autoPtr<faceCoupleInfo> couples =
          determineCoupledFaces
          (
            fullMatch,
            procI,
            masterMesh,
            meshToAdd,
            mergeDist
          );
        // Add elements to mesh
        Info << "Adding to master mesh" << nl << endl;
        autoPtr<mapAddedPolyMesh> map =
          fvMeshAdder::add
          (
            masterMesh,
            meshToAdd,
            couples
          );
        // Update all addressing so xxProcAddressing points to correct
        // item in masterMesh.
        // Processors that were already in masterMesh
        for (label mergedI = 0; mergedI < procI; mergedI++) {
          renumber(map().oldCellMap(), cellProcAddressing[mergedI]);
          renumber(map().oldFaceMap(), faceProcAddressing[mergedI]);
          renumber(map().oldPointMap(), pointProcAddressing[mergedI]);
          // Note: boundary is special since can contain -1.
          renumber(map().oldPatchMap(), boundaryProcAddressing[mergedI]);
        }
        // Added processor
        renumber(map().addedCellMap(), cellProcAddressing[procI]);
        renumber(map().addedFaceMap(), faceProcAddressing[procI]);
        renumber(map().addedPointMap(), pointProcAddressing[procI]);
        renumber(map().addedPatchMap(), boundaryProcAddressing[procI]);
        Info<< endl;
      }
      // See if any points on the mastermesh have become connected
      // because of connections through processor meshes.
      mergeSharedPoints(mergeDist, masterMesh, pointProcAddressing);
      // Save some properties on the reconstructed mesh
      masterInternalFaces = masterMesh.nInternalFaces();
      masterOwner = masterMesh.faceOwner();
      Info << "\nWriting merged mesh to "
        << runTime.path()/runTime.timeName()
        << nl << endl;
      if (!masterMesh.write()) {
        FATAL_ERROR_IN(args.executable())
          << "Failed writing polyMesh."
          << exit(FatalError);
      }
      if (writeCellDist) {
        writeCellDistance(runTime, masterMesh, cellProcAddressing);
      }
    }
    // Write the addressing
    Info << "Reconstructing the addressing from the processor meshes"
      << " to the newly reconstructed mesh" << nl << endl;
    FOR_ALL(databases, procI) {
      Info << "Reading processor " << procI << " mesh from "
        << databases[procI].caseName() << endl;
      polyMesh procMesh
      {
        {
          regionName,
          databases[procI].timeName(),
          databases[procI]
        }
      };
      // From processor point to reconstructed mesh point
      const auto& di = databases[procI];
      Info << "Writing pointProcAddressing to "
        << di.caseName()/procMesh.facesInstance()/polyMesh::meshSubDir
        << endl;
      labelIOList
      {
        {
          "pointProcAddressing",
          procMesh.facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false                       // Do not register
        },
        pointProcAddressing[procI]
      }.write();
      // From processor face to reconstructed mesh face
      Info << "Writing faceProcAddressing to "
        << di.caseName()/procMesh.facesInstance()/polyMesh::meshSubDir
        << endl;
      labelIOList faceProcAddr
      {
        {
          "faceProcAddressing",
          procMesh.facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false                       // Do not register
        },
        faceProcAddressing[procI]
      };
      // Now add turning index to faceProcAddressing.
      // See reconstructPar for meaning of turning index.
      FOR_ALL(faceProcAddr, procFaceI) {
        label masterFaceI = faceProcAddr[procFaceI];
        if (!procMesh.isInternalFace(procFaceI)
            && masterFaceI < masterInternalFaces) {
          // proc face is now external but used to be internal face.
          // Check if we have owner or neighbour.
          label procOwn = procMesh.faceOwner()[procFaceI];
          label masterOwn = masterOwner[masterFaceI];
          if (cellProcAddressing[procI][procOwn] == masterOwn) {
            // No turning. Offset by 1.
            faceProcAddr[procFaceI]++;
          } else {
            // Turned face.
            faceProcAddr[procFaceI] = -1 - faceProcAddr[procFaceI];
          }
        } else {
          // No turning. Offset by 1.
          faceProcAddr[procFaceI]++;
        }
      }
      faceProcAddr.write();
      // From processor cell to reconstructed mesh cell
      Info << "Writing cellProcAddressing to "
        << di.caseName()/procMesh.facesInstance()/polyMesh::meshSubDir << endl;
      labelIOList
      {
        {
          "cellProcAddressing",
          procMesh.facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false                       // Do not register
        },
        cellProcAddressing[procI]
      }.write();
      // From processor patch to reconstructed mesh patch
      Info << "Writing boundaryProcAddressing to "
        << di.caseName()/procMesh.facesInstance()/polyMesh::meshSubDir << endl;
      labelIOList
      {
        {
          "boundaryProcAddressing",
          procMesh.facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        boundaryProcAddressing[procI]
      }.write();
      Info<< endl;
    }
  }
  Info << "End.\n" << endl;
  return 0;
}

