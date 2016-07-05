// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "mesh_to_mesh.hpp"
#include "processor_poly_patch.hpp"
#include "map_meshes.hpp"


void mapConsistentMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const meshToMesh::interpolationMethod& mapMethod,
  const bool subtract,
  const HashSet<word>& selectedFields,
  const bool noLagrangian
)
{
  Info << nl << "Consistently creating and mapping fields for time "
    << meshSource.time().timeName() << nl << endl;
  meshToMesh interp{meshSource, meshTarget, mapMethod};
  if (subtract) {
    MapMesh<minusEqOp>
    (
      interp,
      selectedFields,
      noLagrangian
    );
  } else {
    MapMesh<plusEqOp>
    (
      interp,
      selectedFields,
      noLagrangian
    );
  }
}


void mapSubMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const HashTable<word>& patchMap,
  const wordList& cuttingPatches,
  const meshToMesh::interpolationMethod& mapMethod,
  const bool subtract,
  const HashSet<word>& selectedFields,
  const bool noLagrangian
)
{
  Info << nl << "Creating and mapping fields for time "
    << meshSource.time().timeName() << nl << endl;
  meshToMesh interp
  {
    meshSource,
    meshTarget,
    mapMethod,
    patchMap,
    cuttingPatches
  };
  if (subtract) {
    MapMesh<minusEqOp>
    (
      interp,
      selectedFields,
      noLagrangian
    );
  } else {
    MapMesh<plusEqOp>
    (
      interp,
      selectedFields,
      noLagrangian
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
  HashSet<word> cuttingPatchTable;
  FOR_ALL(cuttingPatches, i) {
    cuttingPatchTable.insert(cuttingPatches[i]);
  }
  const polyBoundaryMesh& pbm = meshTarget.boundaryMesh();
  FOR_ALL(pbm, patchI) {
    if (isA<processorPolyPatch>(pbm[patchI])) {
      const word& patchName = pbm[patchI].name();
      cuttingPatchTable.insert(patchName);
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
  argList::addOption
  (
    "fields",
    "list",
    "specify a list of fields to be mapped. Eg, '(U T p)' - "
    "regular expressions not currently supported"
  );
  argList::addBoolOption
  (
    "noLagrangian",
    "skip mapping lagrangian positions and fields"
  );
  argList args{argc, argv};
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
  const bool consistent = args.optionFound("consistent");
  meshToMesh::interpolationMethod mapMethod = meshToMesh::imCellVolumeWeight;
  if (args.optionFound("mapMethod"))
  {
    mapMethod = meshToMesh::interpolationMethodNames_[args["mapMethod"]];
    Info << "Mapping method: "
      << meshToMesh::interpolationMethodNames_[mapMethod] << endl;
  }
  const bool subtract = args.optionFound("subtract");
  if (subtract) {
    Info << "Subtracting mapped source field from target" << endl;
  }
  HashSet<word> selectedFields;
  if (args.optionFound("fields")) {
    args.optionLookup("fields")() >> selectedFields;
  }
  const bool noLagrangian = args.optionFound("noLagrangian");
  #include "create_times.inc"
  HashTable<word> patchMap;
  wordList cuttingPatches;
  if (!consistent) {
    IOdictionary mapFieldsParDict
    {
      {
        "mapFieldsParDict",
        runTimeTarget.system(),
        runTimeTarget,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    };
    mapFieldsParDict.lookup("patchMap") >> patchMap;
    mapFieldsParDict.lookup("cuttingPatches") >>  cuttingPatches;
  }
  #include "set_time_index.inc"
  Info << "\nCreate meshes\n" << endl;
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
    mapConsistentMesh
    (
      meshSource,
      meshTarget,
      mapMethod,
      subtract,
      selectedFields,
      noLagrangian
    );
  } else {
    mapSubMesh
    (
      meshSource,
      meshTarget,
      patchMap,
      addProcessorPatches(meshTarget, cuttingPatches),
      mapMethod,
      subtract,
      selectedFields,
      noLagrangian
    );
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

