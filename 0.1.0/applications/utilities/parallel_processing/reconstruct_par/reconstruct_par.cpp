// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "fv_cfd.hpp"
#include "ioobject_list.hpp"
#include "processor_meshes.hpp"
#include "region_properties.hpp"
#include "fv_field_reconstructor.hpp"
#include "point_field_reconstructor.hpp"
#include "reconstruct_lagrangian.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"


bool haveAllTimes
(
  const HashSet<word>& masterTimeDirSet,
  const instantList& timeDirs
)
{
  // Loop over all times
  FOR_ALL(timeDirs, timeI) {
    if (!masterTimeDirSet.found(timeDirs[timeI].name())) {
      return false;
    }
  }
  return true;
}


int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Reconstruct fields of a parallel case"
  );
  // Enable -constant ... if someone really wants it
  // Enable -withZero to prevent accidentally trashing the initial fields
  timeSelector::addOptions(true, true);
  argList::noParallel();
  #include "add_region_option.inc"
  argList::addBoolOption
  (
    "allRegions",
    "operate on all regions in regionProperties"
  );
  argList::addOption
  (
    "fields",
    "list",
    "specify a list of fields to be reconstructed. Eg, '(U T p)' - "
    "regular expressions not currently supported"
  );
  argList::addOption
  (
    "lagrangianFields",
    "list",
    "specify a list of lagrangian fields to be reconstructed. Eg, '(U d)' -"
    "regular expressions not currently supported, "
    "positions always included."
  );
  argList::addBoolOption
  (
    "noLagrangian",
    "skip reconstructing lagrangian positions and fields"
  );
  argList::addBoolOption
  (
    "noSets",
    "skip reconstructing cellSets, faceSets, pointSets"
  );
  argList::addBoolOption
  (
    "newTimes",
    "only reconstruct new times (i.e. that do not exist already)"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  HashSet<word> selectedFields;
  if (args.optionFound("fields")) {
    args.optionLookup("fields")() >> selectedFields;
  }
  const bool noLagrangian = args.optionFound("noLagrangian");
  if (noLagrangian) {
    Info << "Skipping reconstructing lagrangian positions and fields"
      << nl << endl;
  }
  const bool noReconstructSets = args.optionFound("noSets");
  if (noReconstructSets) {
    Info << "Skipping reconstructing cellSets, faceSets and pointSets"
      << nl << endl;
  }
  HashSet<word> selectedLagrangianFields;
  if (args.optionFound("lagrangianFields")) {
    if (noLagrangian) {
      FATAL_ERROR_IN(args.executable())
        << "Cannot specify noLagrangian and lagrangianFields "
        << "options together."
        << exit(FatalError);
    }
    args.optionLookup("lagrangianFields")() >> selectedLagrangianFields;
  }
  const bool newTimes   = args.optionFound("newTimes");
  const bool allRegions = args.optionFound("allRegions");
  // determine the processor count directly
  label nProcs = 0;
  while (isDir(args.path()/(word("processor") + name(nProcs)))) {
    ++nProcs;
  }
  if (!nProcs) {
    FATAL_ERROR_IN(args.executable())
      << "No processor* directories found"
      << exit(FatalError);
  }
  // Create the processor databases
  PtrList<Time> databases{nProcs};
  FOR_ALL(databases, procI) {
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
  // Note that we do not set the runTime time so it is still the
  // one set through the controlDict. The -time option
  // only affects the selected set of times from processor0.
  // - can be illogical
  // + any point motion handled through mesh.readUpdate
  if (timeDirs.empty()) {
    FATAL_ERROR_IN(args.executable())
      << "No times selected"
      << exit(FatalError);
  }
  // Get current times if -newTimes
  instantList masterTimeDirs;
  if (newTimes) {
    masterTimeDirs = runTime.times();
  }
  HashSet<word> masterTimeDirSet{2*masterTimeDirs.size()};
  FOR_ALL(masterTimeDirs, i) {
    masterTimeDirSet.insert(masterTimeDirs[i].name());
  }
  // Set all times on processor meshes equal to reconstructed mesh
  FOR_ALL(databases, procI) {
    databases[procI].setTime(runTime);
  }
  wordList regionNames;
  wordList regionDirs;
  if (allRegions) {
    Info << "Reconstructing for all regions in regionProperties" << nl << endl;
    regionProperties rp{runTime};
    FOR_ALL_CONST_ITER(HashTable<wordList>, rp, iter) {
      const wordList& regions = iter();
      FOR_ALL(regions, i) {
        if (findIndex(regionNames, regions[i]) == -1) {
          regionNames.append(regions[i]);
        }
      }
    }
    regionDirs = regionNames;
  } else {
    word regionName;
    if (args.optionReadIfPresent("region", regionName)) {
      regionNames = wordList{1, regionName};
      regionDirs = regionNames;
    } else {
      regionNames = wordList{1, fvMesh::defaultRegion};
      regionDirs = wordList{1, word::null};
    }
  }
  FOR_ALL(regionNames, regionI) {
    const word& regionName = regionNames[regionI];
    const word& regionDir = regionDirs[regionI];
    Info << "\n\nReconstructing fields for mesh " << regionName << nl << endl;
    if (newTimes && regionNames.size() == 1 && regionDirs[0].empty()
        && haveAllTimes(masterTimeDirSet, timeDirs)) {
      Info << "Skipping region " << regionName
        << " since already have all times"
        << endl << endl;
      continue;
    }
    fvMesh mesh
    {
      {
        regionName,
        runTime.timeName(),
        runTime,
        IOobject::MUST_READ
      }
    };
    // Read all meshes and addressing to reconstructed mesh
    processorMeshes procMeshes{databases, regionName};
    // Check face addressing for meshes that have been decomposed
    // with a very old foam version
    #include "check_face_addressing_comp.inc"
    // Loop over all times
    FOR_ALL(timeDirs, timeI) {
      if (newTimes && masterTimeDirSet.found(timeDirs[timeI].name())) {
        Info << "Skipping time " << timeDirs[timeI].name() << nl << endl;
        continue;
      }
      // Set time for global database
      runTime.setTime(timeDirs[timeI], timeI);
      Info<< "Time = " << runTime.timeName() << endl << endl;
      // Set time for all databases
      for (auto& procDb : databases) {
        procDb.setTime(timeDirs[timeI], timeI);
      }
      // Check if any new meshes need to be read.
      fvMesh::readUpdateState meshStat = mesh.readUpdate();
      fvMesh::readUpdateState procStat = procMeshes.readUpdate();
      if (procStat == fvMesh::POINTS_MOVED) {
        // Reconstruct the points for moving mesh cases and write
        // them out
        procMeshes.reconstructPoints(mesh);
      } else if (meshStat != procStat) {
        WARNING_IN(args.executable())
          << "readUpdate for the reconstructed mesh:"
          << meshStat << nl
          << "readUpdate for the processor meshes  :"
          << procStat << nl
          << "These should be equal or your addressing"
          << " might be incorrect."
          << " Please check your time directories for any "
          << "mesh directories." << endl;
      }
      // Get list of objects from processor0 database
      IOobjectList objects
      {
        procMeshes.meshes()[0],
        databases[0].timeName()
      };
      {
        // If there are any FV fields, reconstruct them
        Info << "Reconstructing FV fields" << nl << endl;
        fvFieldReconstructor fvReconstructor
        {
          mesh,
          procMeshes.meshes(),
          procMeshes.faceProcAddressing(),
          procMeshes.cellProcAddressing(),
          procMeshes.boundaryProcAddressing()
        };
        fvReconstructor.reconstructFvVolumeInternalFields<scalar>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeInternalFields<vector>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeInternalFields
        <sphericalTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeInternalFields<symmTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeInternalFields<tensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeFields<scalar>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeFields<vector>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeFields<sphericalTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeFields<symmTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvVolumeFields<tensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvSurfaceFields<scalar>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvSurfaceFields<vector>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvSurfaceFields<sphericalTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvSurfaceFields<symmTensor>
        (
          objects,
          selectedFields
        );
        fvReconstructor.reconstructFvSurfaceFields<tensor>
        (
          objects,
          selectedFields
        );
        if (fvReconstructor.nReconstructed() == 0) {
          Info << "No FV fields" << nl << endl;
        }
      }
      {
        Info << "Reconstructing point fields" << nl << endl;
        const pointMesh& pMesh = pointMesh::New(mesh);
        PtrList<pointMesh> pMeshes{procMeshes.meshes().size()};
        FOR_ALL(pMeshes, procI) {
          pMeshes.set
          (
            procI,
            new pointMesh{procMeshes.meshes()[procI]}
          );
        }
        pointFieldReconstructor pointReconstructor
        {
          pMesh,
          pMeshes,
          procMeshes.pointProcAddressing(),
          procMeshes.boundaryProcAddressing()
        };
        pointReconstructor.reconstructFields<scalar>(objects, selectedFields);
        pointReconstructor.reconstructFields<vector>(objects, selectedFields);
        pointReconstructor.reconstructFields<sphericalTensor>
        (
          objects,
          selectedFields
        );
        pointReconstructor.reconstructFields<symmTensor>
        (
          objects,
          selectedFields
        );
        pointReconstructor.reconstructFields<tensor>
        (
          objects,
          selectedFields
        );
        if (pointReconstructor.nReconstructed() == 0) {
          Info << "No point fields" << nl << endl;
        }
      }
      // If there are any clouds, reconstruct them.
      // The problem is that a cloud of size zero will not get written so
      // in pass 1 we determine the cloud names and per cloud name the
      // fields. Note that the fields are stored as IOobjectList from
      // the first processor that has them. They are in pass2 only used
      // for name and type (scalar, vector etc).
      if (!noLagrangian) {
        HashTable<IOobjectList> cloudObjects;
        FOR_ALL(databases, procI) {
          fileNameList cloudDirs
          {
            readDir(databases[procI].timePath()/regionDir/cloud::prefix,
                    fileName::DIRECTORY)
          };
          FOR_ALL(cloudDirs, i) {
            // Check if we already have cloud objects for this
            // cloudname
            HashTable<IOobjectList>::const_iterator iter =
              cloudObjects.find(cloudDirs[i]);
            if (iter == cloudObjects.end()) {
              // Do local scan for valid cloud objects
              IOobjectList sprayObjs
              {
                procMeshes.meshes()[procI],
                databases[procI].timeName(),
                cloud::prefix/cloudDirs[i]
              };
              IOobject* positionsPtr = sprayObjs.lookup(word("positions"));
              if (positionsPtr) {
                cloudObjects.insert(cloudDirs[i], sprayObjs);
              }
            }
          }
        }
        if (cloudObjects.size()) {
          // Pass2: reconstruct the cloud
          FOR_ALL_CONST_ITER(HashTable<IOobjectList>, cloudObjects, iter) {
            const word cloudName = string::validate<word>(iter.key());
            // Objects (on arbitrary processor)
            const IOobjectList& sprayObjs = iter();
            Info<< "Reconstructing lagrangian fields for cloud "
              << cloudName << nl << endl;
            reconstructLagrangianPositions
            (
              mesh,
              cloudName,
              procMeshes.meshes(),
              procMeshes.faceProcAddressing(),
              procMeshes.cellProcAddressing()
            );
            reconstructLagrangianFields<label>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<label>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFields<scalar>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<scalar>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFields<vector>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<vector>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFields<sphericalTensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<sphericalTensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFields<symmTensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<symmTensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFields<tensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
            reconstructLagrangianFieldFields<tensor>
            (
              cloudName,
              mesh,
              procMeshes.meshes(),
              sprayObjs,
              selectedLagrangianFields
            );
          }
        } else {
          Info << "No lagrangian fields" << nl << endl;
        }
      }
      if (!noReconstructSets) {
        // Scan to find all sets
        HashTable<label> cSetNames;
        HashTable<label> fSetNames;
        HashTable<label> pSetNames;
        FOR_ALL(procMeshes.meshes(), procI) {
          const fvMesh& procMesh = procMeshes.meshes()[procI];
          // Note: look at sets in current time only or between
          // mesh and current time?. For now current time. This will
          // miss out on sets in intermediate times that have not
          // been reconstructed.
          IOobjectList objects
          {
            procMesh,
            databases[0].timeName(),    //procMesh.facesInstance()
            polyMesh::meshSubDir/"sets"
          };
          IOobjectList cSets{objects.lookupClass(cellSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, cSets, iter) {
            cSetNames.insert(iter.key(), cSetNames.size());
          }
          IOobjectList fSets{objects.lookupClass(faceSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, fSets, iter) {
            fSetNames.insert(iter.key(), fSetNames.size());
          }
          IOobjectList pSets{objects.lookupClass(pointSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, pSets, iter) {
            pSetNames.insert(iter.key(), pSetNames.size());
          }
        }
        // Construct all sets
        PtrList<cellSet> cellSets{cSetNames.size()};
        PtrList<faceSet> faceSets{fSetNames.size()};
        PtrList<pointSet> pointSets{pSetNames.size()};
        Info << "Reconstructing sets:" << endl;
        if (cSetNames.size()) {
          Info << "    cellSets " << cSetNames.sortedToc() << endl;
        }
        if (fSetNames.size()) {
          Info << "    faceSets " << fSetNames.sortedToc() << endl;
        }
        if (pSetNames.size()) {
          Info << "    pointSets " << pSetNames.sortedToc() << endl;
        }
        // Load sets
        FOR_ALL(procMeshes.meshes(), procI) {
          const fvMesh& procMesh = procMeshes.meshes()[procI];
          IOobjectList objects
          {
            procMesh,
            databases[0].timeName(),    //procMesh.facesInstance(),
            polyMesh::meshSubDir/"sets"
          };
          // cellSets
          const labelList& cellMap = procMeshes.cellProcAddressing()[procI];
          IOobjectList cSets{objects.lookupClass(cellSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, cSets, iter) {
            // Load cellSet
            const cellSet procSet(*iter());
            label setI = cSetNames[iter.key()];
            if (!cellSets.set(setI)) {
              cellSets.set
              (
                setI,
                new cellSet{mesh, iter.key(), procSet.size()}
              );
            }
            cellSet& cSet = cellSets[setI];
            FOR_ALL_CONST_ITER(cellSet, procSet, iter) {
              cSet.insert(cellMap[iter.key()]);
            }
          }
          // faceSets
          const labelList& faceMap = procMeshes.faceProcAddressing()[procI];
          IOobjectList fSets{objects.lookupClass(faceSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, fSets, iter) {
            // Load faceSet
            const faceSet procSet{*iter()};
            label setI = fSetNames[iter.key()];
            if (!faceSets.set(setI)) {
              faceSets.set
              (
                setI,
                new faceSet{mesh, iter.key(), procSet.size()}
              );
            }
            faceSet& fSet = faceSets[setI];
            FOR_ALL_CONST_ITER(faceSet, procSet, iter) {
              fSet.insert(mag(faceMap[iter.key()])-1);
            }
          }
          // pointSets
          const labelList& pointMap = procMeshes.pointProcAddressing()[procI];
          IOobjectList pSets{objects.lookupClass(pointSet::typeName)};
          FOR_ALL_CONST_ITER(IOobjectList, pSets, iter) {
            // Load pointSet
            const pointSet propSet{*iter()};
            label setI = pSetNames[iter.key()];
            if (!pointSets.set(setI)) {
              pointSets.set
              (
                setI,
                new pointSet{mesh, iter.key(), propSet.size()}
              );
            }
            pointSet& pSet = pointSets[setI];
            FOR_ALL_CONST_ITER(pointSet, propSet, iter) {
              pSet.insert(pointMap[iter.key()]);
            }
          }
        }
        // Write sets
        FOR_ALL(cellSets, i) {
          cellSets[i].write();
        }
        FOR_ALL(faceSets, i) {
          faceSets[i].write();
        }
        FOR_ALL(pointSets, i) {
          pointSets[i].write();
        }
      }
    }
  }
  // If there are any "uniform" directories copy them from
  // the master processor
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    databases[0].setTime(timeDirs[timeI], timeI);
    fileName uniformDir0 = databases[0].timePath()/"uniform";
    if (isDir(uniformDir0)) {
      cp(uniformDir0, runTime.timePath());
    }
  }
  Info << "End.\n" << endl;
  return 0;
}

