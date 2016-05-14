// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "mesh_to_mesh0.hpp"
#include "processor_fv_patch.hpp"
#include "map_meshes.hpp"


void mapConsistentMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const meshToMesh0::order& mapOrder,
  const bool subtract
)
{
  if (subtract) {
    MapConsistentMesh<minusEqOp>
    (
      meshSource,
      meshTarget,
      mapOrder
    );
  } else {
    MapConsistentMesh<eqOp>
    (
      meshSource,
      meshTarget,
      mapOrder
    );
  }
}


void mapSubMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const HashTable<word>& patchMap,
  const wordList& cuttingPatches,
  const meshToMesh0::order& mapOrder,
  const bool subtract
)
{
  if (subtract) {
    MapSubMesh<minusEqOp>
    (
      meshSource,
      meshTarget,
      patchMap,
      cuttingPatches,
      mapOrder
    );
  } else {
    MapSubMesh<eqOp>
    (
      meshSource,
      meshTarget,
      patchMap,
      cuttingPatches,
      mapOrder
    );
  }
}


void mapConsistentSubMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const meshToMesh0::order& mapOrder,
  const bool subtract
)
{
  if (subtract) {
    MapConsistentSubMesh<minusEqOp>
    (
      meshSource,
      meshTarget,
      mapOrder
    );
  } else {
    MapConsistentSubMesh<eqOp>
    (
      meshSource,
      meshTarget,
      mapOrder
    );
  }
}


wordList addProcessorPatches
(
  const fvMesh& meshTarget,
  const wordList& cuttingPatches
)
{
  // Add the processor patches to the cutting list
  HashTable<label> cuttingPatchTable;
  FOR_ALL(cuttingPatches, i) {
    cuttingPatchTable.insert(cuttingPatches[i], i);
  }
  FOR_ALL(meshTarget.boundary(), patchi) {
    if (!isA<processorFvPatch>(meshTarget.boundary()[patchi]))
      continue;
    if (!cuttingPatchTable.found
        (
          meshTarget.boundaryMesh()[patchi].name()
        )) {
      cuttingPatchTable.insert
      (
        meshTarget.boundaryMesh()[patchi].name(),
        -1
      );
    }
  }
  return cuttingPatchTable.toc();
}


int main(int argc, char *argv[])
{
  argList::addNote
  (
    "map volume fields from one mesh to another"
  );
  argList::noParallel();
  argList::validArgs.append("sourceCase");
  argList::addOption
  (
    "sourceTime",
    "scalar|'latestTime'",
    "specify the source time"
  );
  argList::addOption
  (
    "sourceRegion",
    "word",
    "specify the source region"
  );
  argList::addOption
  (
    "targetRegion",
    "word",
    "specify the target region"
  );
  argList::addBoolOption
  (
    "parallelSource",
    "the source is decomposed"
  );
  argList::addBoolOption
  (
    "parallelTarget",
    "the target is decomposed"
  );
  argList::addBoolOption
  (
    "consistent",
    "source and target geometry and boundary conditions identical"
  );
  argList::addOption
  (
    "mapMethod",
    "word",
    "specify the mapping method"
  );
  argList::addBoolOption
  (
    "subtract",
    "subtract mapped source from target"
  );
  argList args{argc, argv};
  if (!args.check()) {
    FatalError.exit();
  }
  fileName rootDirTarget{args.rootPath()};
  fileName caseDirTarget{args.globalCaseName()};
  const fileName casePath = args[1];
  const fileName rootDirSource = casePath.path();
  const fileName caseDirSource = casePath.name();
  Info << "Source: " << rootDirSource << " " << caseDirSource << endl;
  word sourceRegion = fvMesh::defaultRegion;
  if (args.optionFound("sourceRegion")) {
    sourceRegion = args["sourceRegion"];
    Info << "Source region: " << sourceRegion << endl;
  }
  Info << "Target: " << rootDirTarget << " " << caseDirTarget << endl;
  word targetRegion = fvMesh::defaultRegion;
  if (args.optionFound("targetRegion")) {
    targetRegion = args["targetRegion"];
    Info << "Target region: " << targetRegion << endl;
  }
  const bool parallelSource = args.optionFound("parallelSource");
  const bool parallelTarget = args.optionFound("parallelTarget");
  const bool consistent = args.optionFound("consistent");
  meshToMesh0::order mapOrder = meshToMesh0::INTERPOLATE;
  if (args.optionFound("mapMethod")) {
    const word mapMethod(args["mapMethod"]);
    if (mapMethod == "mapNearest") {
      mapOrder = meshToMesh0::MAP;
    } else if (mapMethod == "interpolate") {
      mapOrder = meshToMesh0::INTERPOLATE;
    } else if (mapMethod == "cellPointInterpolate") {
      mapOrder = meshToMesh0::CELL_POINT_INTERPOLATE;
    } else {
      FATAL_ERROR_IN(args.executable())
        << "Unknown mapMethod " << mapMethod << ". Valid options are: "
        << "mapNearest, interpolate and cellPointInterpolate"
        << exit(FatalError);
    }
    Info << "Mapping method: " << mapMethod << endl;
  }
  const bool subtract = args.optionFound("subtract");
  if (subtract) {
    Info << "Subtracting mapped source field from target" << endl;
  }
  #include "create_times.inc"
  HashTable<word> patchMap;
  wordList cuttingPatches;
  if (!consistent) {
    IOdictionary mapFieldsDict
    {
      // IOobject
      {
        "mapFieldsDict",
        runTimeTarget.system(),
        runTimeTarget,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    };
    mapFieldsDict.lookup("patchMap") >> patchMap;
    mapFieldsDict.lookup("cuttingPatches") >>  cuttingPatches;
  }
  if (parallelSource && !parallelTarget) {
    IOdictionary decompositionDict
    {
      // IOobject
      {
        "decomposeParDict",
        runTimeSource.system(),
        runTimeSource,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    int nProcs{readInt(decompositionDict.lookup("numberOfSubdomains"))};
    Info << "Create target mesh\n" << endl;
    fvMesh meshTarget
    {
      // IOobject
      {
        targetRegion,
        runTimeTarget.timeName(),
        runTimeTarget
      }
    };
    Info << "Target mesh size: " << meshTarget.nCells() << endl;
    for (int procI=0; procI<nProcs; procI++) {
      Info << nl << "Source processor " << procI << endl;
      Time runTimeSource
      {
        Time::controlDictName,
        rootDirSource,
        caseDirSource/fileName(word("processor") + name(procI))
      };
      #include "set_time_index.inc"
      fvMesh meshSource
      {
        // IOobject
        {
          sourceRegion,
          runTimeSource.timeName(),
          runTimeSource
        }
      };
      Info << "mesh size: " << meshSource.nCells() << endl;
      if (consistent) {
        mapConsistentSubMesh
        (
          meshSource,
          meshTarget,
          mapOrder,
          subtract
        );
      } else {
        mapSubMesh
        (
          meshSource,
          meshTarget,
          patchMap,
          cuttingPatches,
          mapOrder,
          subtract
        );
      }
    }
  } else if (!parallelSource && parallelTarget) {
    IOdictionary decompositionDict
    {
      {
        "decomposeParDict",
        runTimeTarget.system(),
        runTimeTarget,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    int nProcs{readInt(decompositionDict.lookup("numberOfSubdomains"))};
    Info << "Create source mesh\n" << endl;
    #include "set_time_index.inc"
    fvMesh meshSource
    {
      {
        sourceRegion,
        runTimeSource.timeName(),
        runTimeSource
      }
    };
    Info << "Source mesh size: " << meshSource.nCells() << endl;
    for (int procI=0; procI<nProcs; procI++) {
      Info << nl << "Target processor " << procI << endl;
      Time runTimeTarget
      {
        Time::controlDictName,
        rootDirTarget,
        caseDirTarget/fileName(word("processor") + name(procI))
      };
      fvMesh meshTarget
      {
        {
          targetRegion,
          runTimeTarget.timeName(),
          runTimeTarget
        }
      };
      Info << "mesh size: " << meshTarget.nCells() << endl;
      if (consistent) {
        mapConsistentSubMesh
        (
          meshSource,
          meshTarget,
          mapOrder,
          subtract
        );
      } else {
        mapSubMesh
        (
          meshSource,
          meshTarget,
          patchMap,
          addProcessorPatches(meshTarget, cuttingPatches),
          mapOrder,
          subtract
        );
      }
    }
  } else if (parallelSource && parallelTarget) {
    IOdictionary decompositionDictSource
    {
      {
        "decomposeParDict",
        runTimeSource.system(),
        runTimeSource,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    int nProcsSource
    {
      readInt(decompositionDictSource.lookup("numberOfSubdomains"))
    };
    IOdictionary decompositionDictTarget
    {
      {
        "decomposeParDict",
        runTimeTarget.system(),
        runTimeTarget,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    int nProcsTarget
    {
      readInt(decompositionDictTarget.lookup("numberOfSubdomains"))
    };
    List<boundBox> bbsTarget{nProcsTarget};
    List<bool> bbsTargetSet{nProcsTarget, false};
    for (int procISource=0; procISource<nProcsSource; procISource++) {
      Info << nl << "Source processor " << procISource << endl;
      Time runTimeSource
      {
        Time::controlDictName,
        rootDirSource,
        caseDirSource/fileName(word("processor") + name(procISource))
      };
      #include "set_time_index.inc"
      fvMesh meshSource
      {
        {
          sourceRegion,
          runTimeSource.timeName(),
          runTimeSource
        }
      };
      Info << "mesh size: " << meshSource.nCells() << endl;
      boundBox bbSource{meshSource.bounds()};
      for (int procITarget=0; procITarget<nProcsTarget; procITarget++) {
        if (!bbsTargetSet[procITarget] 
            || (bbsTargetSet[procITarget]
                && bbsTarget[procITarget].overlaps(bbSource))) {
          Info << nl << "Target processor " << procITarget << endl;
          Time runTimeTarget
          {
            Time::controlDictName,
            rootDirTarget,
            caseDirTarget/fileName(word("processor") + name(procITarget))
          };
          fvMesh meshTarget
          {
            {
              targetRegion,
              runTimeTarget.timeName(),
              runTimeTarget
            }
          };
          Info << "mesh size: " << meshTarget.nCells() << endl;
          bbsTarget[procITarget] = meshTarget.bounds();
          bbsTargetSet[procITarget] = true;
          if (bbsTarget[procITarget].overlaps(bbSource)) {
            if (consistent) {
              mapConsistentSubMesh
              (
                meshSource,
                meshTarget,
                mapOrder,
                subtract
              );
            } else {
              mapSubMesh
              (
                meshSource,
                meshTarget,
                patchMap,
                addProcessorPatches(meshTarget, cuttingPatches),
                mapOrder,
                subtract
              );
            }
          }
        }
      }
    }
  } else {
    #include "set_time_index.inc"
    Info << "Create meshes\n" << endl;
    fvMesh meshSource
    {
      {
        sourceRegion,
        runTimeSource.timeName(),
        runTimeSource
      }
    };
    fvMesh meshTarget
    {
      {
        targetRegion,
        runTimeTarget.timeName(),
        runTimeTarget
      }
    };
    Info << "Source mesh size: " << meshSource.nCells() << tab
      << "Target mesh size: " << meshTarget.nCells() << nl << endl;
    if (consistent) {
      mapConsistentMesh(meshSource, meshTarget, mapOrder, subtract);
    } else {
      mapSubMesh
      (
        meshSource,
        meshTarget,
        patchMap,
        cuttingPatches,
        mapOrder,
        subtract
      );
    }
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

