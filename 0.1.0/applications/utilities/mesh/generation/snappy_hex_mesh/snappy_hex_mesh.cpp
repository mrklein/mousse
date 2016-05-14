// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "auto_refine_driver.hpp"
#include "auto_snap_driver.hpp"
#include "auto_layer_driver.hpp"
#include "searchable_surfaces.hpp"
#include "refinement_surfaces.hpp"
#include "refinement_features.hpp"
#include "shell_surfaces.hpp"
#include "decomposition_method.hpp"
#include "no_decomp.hpp"
#include "fv_mesh_distribute.hpp"
#include "wall_poly_patch.hpp"
#include "refinement_parameters.hpp"
#include "snap_parameters.hpp"
#include "layer_parameters.hpp"
#include "vtk_set_writer.hpp"
#include "face_set.hpp"
#include "motion_smoother.hpp"
#include "poly_topo_change.hpp"
#include "cell_modeller.hpp"
#include "uindirect_primitive_patch.hpp"
#include "surf_zone_identifier_list.hpp"
#include "unsorted_meshed_surface.hpp"
#include "meshed_surface.hpp"
#include "global_index.hpp"
#include "iomanip.hpp"


using namespace mousse;


// Convert size (as fraction of defaultCellSize) to refinement level
label sizeCoeffToRefinement
(
  const scalar level0Coeff,   // ratio of hex cell size v.s. defaultCellSize
  const scalar sizeCoeff
)
{
  return round(::log(level0Coeff/sizeCoeff)/::log(2));
}


autoPtr<refinementSurfaces> createRefinementSurfaces
(
  const searchableSurfaces& allGeometry,
  const dictionary& surfacesDict,
  const dictionary& /*shapeControlDict*/,
  const label gapLevelIncrement,
  const scalar level0Coeff
)
{
  autoPtr<refinementSurfaces> surfacePtr;
  // Count number of surfaces.
  label surfI = 0;
  FOR_ALL(allGeometry.names(), geomI) {
    const word& geomName = allGeometry.names()[geomI];
    if (surfacesDict.found(geomName)) {
      surfI++;
    }
  }
  labelList surfaces{surfI};
  wordList names{surfI};
  PtrList<surfaceZonesInfo> surfZones{surfI};
  labelList regionOffset{surfI};
  labelList globalMinLevel{surfI, 0};
  labelList globalMaxLevel{surfI, 0};
  labelList globalLevelIncr{surfI, 0};
  PtrList<dictionary> globalPatchInfo{surfI};
  List<Map<label>> regionMinLevel{surfI};
  List<Map<label>> regionMaxLevel{surfI};
  List<Map<label>> regionLevelIncr{surfI};
  List<Map<scalar>> regionAngle{surfI};
  List<Map<autoPtr<dictionary>>> regionPatchInfo{surfI};
  HashSet<word> unmatchedKeys{surfacesDict.toc()};
  surfI = 0;
  FOR_ALL(allGeometry.names(), geomI) {
    const word& geomName = allGeometry.names()[geomI];
    const entry* ePtr = surfacesDict.lookupEntryPtr(geomName, false, true);
    if (!ePtr)
      continue;
    const dictionary& shapeDict = ePtr->dict();
    unmatchedKeys.erase(ePtr->keyword());
    names[surfI] = geomName;
    surfaces[surfI] = geomI;
    const searchableSurface& surface = allGeometry[geomI];
    // Find the index in shapeControlDict
    // Invert surfaceCellSize to get the refinementLevel
    const word scsFuncName =
      shapeDict.lookup("surfaceCellSizeFunction");
    const dictionary& scsDict =
      shapeDict.subDict(scsFuncName + "Coeffs");
    const scalar surfaceCellSize =
      readScalar(scsDict.lookup("surfaceCellSizeCoeff"));
    const label refLevel =
      sizeCoeffToRefinement(level0Coeff, surfaceCellSize);
    globalMinLevel[surfI] = refLevel;
    globalMaxLevel[surfI] = refLevel;
    globalLevelIncr[surfI] = gapLevelIncrement;
    // Surface zones
    surfZones.set(surfI, new surfaceZonesInfo{surface, shapeDict});
    // Global perpendicular angle
    if (shapeDict.found("patchInfo")) {
      globalPatchInfo.set
      (
        surfI,
        shapeDict.subDict("patchInfo").clone()
      );
    }
    // Per region override of patchInfo
    if (shapeDict.found("regions")) {
      const dictionary& regionsDict = shapeDict.subDict("regions");
      const wordList& regionNames =
        allGeometry[surfaces[surfI]].regions();
      FOR_ALL(regionNames, regionI) {
        if (!regionsDict.found(regionNames[regionI]))
          continue;
        // Get the dictionary for region
        const dictionary& regionDict =
          regionsDict.subDict(regionNames[regionI]);
        if (!regionDict.found("patchInfo"))
          continue;
        regionPatchInfo[surfI].insert
        (
          regionI,
          regionDict.subDict("patchInfo").clone()
        );
      }
    }
    // Per region override of cellSize
    if (shapeDict.found("regions")) {
      const dictionary& shapeControlRegionsDict =
        shapeDict.subDict("regions");
      const wordList& regionNames =
        allGeometry[surfaces[surfI]].regions();
      FOR_ALL(regionNames, regionI) {
        if (!shapeControlRegionsDict.found(regionNames[regionI]))
          continue;
        const dictionary& shapeControlRegionDict =
          shapeControlRegionsDict.subDict(regionNames[regionI]);
        const word scsFuncName =
          shapeControlRegionDict.lookup("surfaceCellSizeFunction");
        const dictionary& scsDict =
          shapeControlRegionDict.subDict(scsFuncName + "Coeffs");
        const scalar surfaceCellSize =
          readScalar(scsDict.lookup("surfaceCellSizeCoeff"));
        const label refLevel =
          sizeCoeffToRefinement(level0Coeff, surfaceCellSize);
        regionMinLevel[surfI].insert(regionI, refLevel);
        regionMaxLevel[surfI].insert(regionI, refLevel);
        regionLevelIncr[surfI].insert(regionI, 0);
      }
    }
    surfI++;
  }
  // Calculate local to global region offset
  label nRegions = 0;
  FOR_ALL(surfaces, surfI) {
    regionOffset[surfI] = nRegions;
    nRegions += allGeometry[surfaces[surfI]].regions().size();
  }
  // Rework surface specific information into information per global region
  labelList minLevel{nRegions, 0};
  labelList maxLevel{nRegions, 0};
  labelList gapLevel{nRegions, -1};
  PtrList<dictionary> patchInfo{nRegions};
  FOR_ALL(globalMinLevel, surfI) {
    label nRegions = allGeometry[surfaces[surfI]].regions().size();
    // Initialise to global (i.e. per surface)
    for (label i = 0; i < nRegions; i++) {
      label globalRegionI = regionOffset[surfI] + i;
      minLevel[globalRegionI] = globalMinLevel[surfI];
      maxLevel[globalRegionI] = globalMaxLevel[surfI];
      gapLevel[globalRegionI] = maxLevel[globalRegionI] + globalLevelIncr[surfI];
      if (globalPatchInfo.set(surfI)) {
        patchInfo.set
        (
          globalRegionI,
          globalPatchInfo[surfI].clone()
        );
      }
    }
    // Overwrite with region specific information
    FOR_ALL_CONST_ITER(Map<label>, regionMinLevel[surfI], iter) {
      label globalRegionI = regionOffset[surfI] + iter.key();
      minLevel[globalRegionI] = iter();
      maxLevel[globalRegionI] = regionMaxLevel[surfI][iter.key()];
      gapLevel[globalRegionI] =
        maxLevel[globalRegionI] + regionLevelIncr[surfI][iter.key()];
    }
    const Map<autoPtr<dictionary>>& localInfo = regionPatchInfo[surfI];
    FOR_ALL_CONST_ITER(Map<autoPtr<dictionary>>, localInfo, iter) {
      label globalRegionI = regionOffset[surfI] + iter.key();
      patchInfo.set(globalRegionI, iter()().clone());
    }
  }
  surfacePtr.set
  (
    new refinementSurfaces
    {
      allGeometry,
      surfaces,
      names,
      surfZones,
      regionOffset,
      minLevel,
      maxLevel,
      gapLevel,
      scalarField{nRegions, -GREAT},  //perpendicularAngle,
      patchInfo
    }
  );
  const refinementSurfaces& rf = surfacePtr();
  // Determine maximum region name length
  label maxLen = 0;
  FOR_ALL(rf.surfaces(), surfI) {
    label geomI = rf.surfaces()[surfI];
    const wordList& regionNames = allGeometry.regionNames()[geomI];
    FOR_ALL(regionNames, regionI) {
      maxLen = mousse::max(maxLen, label(regionNames[regionI].size()));
    }
  }
  Info << setw(maxLen) << "Region"
    << setw(10) << "Min Level"
    << setw(10) << "Max Level"
    << setw(10) << "Gap Level" << nl
    << setw(maxLen) << "------"
    << setw(10) << "---------"
    << setw(10) << "---------"
    << setw(10) << "---------" << endl;
  FOR_ALL(rf.surfaces(), surfI) {
    label geomI = rf.surfaces()[surfI];
    Info << rf.names()[surfI] << ':' << nl;
    const wordList& regionNames = allGeometry.regionNames()[geomI];
    FOR_ALL(regionNames, regionI) {
      label globalI = rf.globalRegion(surfI, regionI);
      Info << setw(maxLen) << regionNames[regionI]
        << setw(10) << rf.minLevel()[globalI]
        << setw(10) << rf.maxLevel()[globalI]
        << setw(10) << rf.gapLevel()[globalI] << endl;
    }
  }
  return surfacePtr;
}


void extractSurface
(
  const polyMesh& mesh,
  const Time& runTime,
  const labelHashSet& includePatches,
  const fileName& outFileName
)
{
  const polyBoundaryMesh& bMesh = mesh.boundaryMesh();
  // Collect sizes. Hash on names to handle local-only patches (e.g.
  //  processor patches)
  HashTable<label> patchSize{1000};
  label nFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, includePatches, iter) {
    const polyPatch& pp = bMesh[iter.key()];
    patchSize.insert(pp.name(), pp.size());
    nFaces += pp.size();
  }
  Pstream::mapCombineGather(patchSize, plusEqOp<label>());
  // Allocate zone/patch for all patches
  HashTable<label> compactZoneID{1000};
  FOR_ALL_CONST_ITER(HashTable<label>, patchSize, iter) {
    label sz = compactZoneID.size();
    compactZoneID.insert(iter.key(), sz);
  }
  Pstream::mapCombineScatter(compactZoneID);
  // Rework HashTable into labelList just for speed of conversion
  labelList patchToCompactZone{bMesh.size(), -1};
  FOR_ALL_CONST_ITER(HashTable<label>, compactZoneID, iter) {
    label patchI = bMesh.findPatchID(iter.key());
    if (patchI != -1) {
      patchToCompactZone[patchI] = iter();
    }
  }
  // Collect faces on zones
  DynamicList<label> faceLabels{nFaces};
  DynamicList<label> compactZones{nFaces};
  FOR_ALL_CONST_ITER(labelHashSet, includePatches, iter) {
    const polyPatch& pp = bMesh[iter.key()];
    FOR_ALL(pp, i) {
      faceLabels.append(pp.start()+i);
      compactZones.append(patchToCompactZone[pp.index()]);
    }
  }
  // Addressing engine for all faces
  uindirectPrimitivePatch allBoundary
  {
    UIndirectList<face>{mesh.faces(), faceLabels},
    mesh.points()
  };
  // Find correspondence to master points
  labelList pointToGlobal;
  labelList uniqueMeshPoints;
  autoPtr<globalIndex> globalNumbers =
    mesh.globalData().mergePoints
    (
      allBoundary.meshPoints(),
      allBoundary.meshPointMap(),
      pointToGlobal,
      uniqueMeshPoints
    );
  // Gather all unique points on master
  List<pointField> gatheredPoints{Pstream::nProcs()};
  gatheredPoints[Pstream::myProcNo()] =
    pointField
    {
      mesh.points(),
      uniqueMeshPoints
    };
  Pstream::gatherList(gatheredPoints);
  // Gather all faces
  List<faceList> gatheredFaces{Pstream::nProcs()};
  gatheredFaces[Pstream::myProcNo()] = allBoundary.localFaces();
  FOR_ALL(gatheredFaces[Pstream::myProcNo()], i) {
    inplaceRenumber(pointToGlobal, gatheredFaces[Pstream::myProcNo()][i]);
  }
  Pstream::gatherList(gatheredFaces);
  // Gather all ZoneIDs
  List<labelList> gatheredZones{Pstream::nProcs()};
  gatheredZones[Pstream::myProcNo()] = compactZones.xfer();
  Pstream::gatherList(gatheredZones);
  // On master combine all points, faces, zones
  if (Pstream::master()) {
    pointField allPoints =
      ListListOps::combine<pointField>
      (
        gatheredPoints,
        accessOp<pointField>()
      );
    gatheredPoints.clear();
    faceList allFaces =
      ListListOps::combine<faceList>
      (
        gatheredFaces,
        accessOp<faceList>()
      );
    gatheredFaces.clear();
    labelList allZones =
      ListListOps::combine<labelList>
      (
        gatheredZones,
        accessOp<labelList>()
      );
    gatheredZones.clear();
    // Zones
    surfZoneIdentifierList surfZones{compactZoneID.size()};
    FOR_ALL_CONST_ITER(HashTable<label>, compactZoneID, iter) {
      surfZones[iter()] = surfZoneIdentifier(iter.key(), iter());
      Info << "surfZone " << iter()  <<  " : " << surfZones[iter()].name()
        << endl;
    }
    UnsortedMeshedSurface<face> unsortedFace
    {
      xferMove(allPoints),
      xferMove(allFaces),
      xferMove(allZones),
      xferMove(surfZones)
    };
    MeshedSurface<face> sortedFace{unsortedFace};
    fileName globalCasePath
    {
      runTime.processorCase()
      ? runTime.path()/".."/outFileName
      : runTime.path()/outFileName
    };
    Info << "Writing merged surface to " << globalCasePath << endl;
    sortedFace.write(globalCasePath);
  }
}


// Check writing tolerance before doing any serious work
scalar getMergeDistance(const polyMesh& mesh, const scalar mergeTol)
{
  const boundBox& meshBb = mesh.bounds();
  scalar mergeDist = mergeTol * meshBb.mag();
  Info << nl
    << "Overall mesh bounding box  : " << meshBb << nl
    << "Relative tolerance         : " << mergeTol << nl
    << "Absolute matching distance : " << mergeDist << nl
    << endl;
  // check writing tolerance
  if (mesh.time().writeFormat() == IOstream::ASCII) {
    const scalar writeTol =
      std::pow(scalar(10.0), -scalar(IOstream::defaultPrecision()));
    if (mergeTol < writeTol) {
      FATAL_ERROR_IN("getMergeDistance(const polyMesh&, const dictionary&)")
        << "Your current settings specify ASCII writing with "
        << IOstream::defaultPrecision() << " digits precision." << nl
        << "Your merging tolerance (" << mergeTol
        << ") is finer than this." << nl
        << "Change to binary writeFormat, "
        << "or increase the writePrecision" << endl
        << "or adjust the merge tolerance (mergeTol)."
        << exit(FatalError);
    }
  }
  return mergeDist;
}


// Write mesh and additional information
void writeMesh
(
  const string& msg,
  const meshRefinement& meshRefiner,
  const meshRefinement::debugType debugLevel,
  const meshRefinement::writeType writeLevel
)
{
  const fvMesh& mesh = meshRefiner.mesh();
  meshRefiner.printMeshInfo(debugLevel, msg);
  Info << "Writing mesh to time " << meshRefiner.timeName() << endl;
  meshRefiner.write
  (
    debugLevel,
    meshRefinement::writeType(writeLevel | meshRefinement::WRITEMESH),
    mesh.time().path()/meshRefiner.timeName()
  );
  Info << "Wrote mesh in = "
    << mesh.time().cpuTimeIncrement() << " s." << endl;
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  mousse::argList::addBoolOption
  (
    "checkGeometry",
    "check all surface geometry for quality"
  );
  mousse::argList::addOption
  (
    "surfaceSimplify",
    "boundBox",
    "simplify the surface using snappyHexMesh starting from a boundBox"
  );
  mousse::argList::addOption
  (
    "patches",
    "(patch0 .. patchN)",
    "only triangulate selected patches (wildcards supported)"
  );
  mousse::argList::addOption
  (
    "outFile",
    "fileName",
    "name of the file to save the simplified surface to"
  );
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  const bool overwrite = args.optionFound("overwrite");
  const bool checkGeometry = args.optionFound("checkGeometry");
  const bool surfaceSimplify = args.optionFound("surfaceSimplify");
  autoPtr<fvMesh> meshPtr;

  {
    mousse::Info
      << "Create mesh for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
    meshPtr.set
    (
      new fvMesh
      {
        // IOobject
        {
          fvMesh::defaultRegion,
          runTime.timeName(),
          runTime,
          mousse::IOobject::MUST_READ
        }
      }
    );
  }
  fvMesh& mesh = meshPtr();
  Info << "Read mesh in = "
    << runTime.cpuTimeIncrement() << " s" << endl;
  // Check patches and faceZones are synchronised
  mesh.boundaryMesh().checkParallelSync(true);
  meshRefinement::checkCoupledFaceZones(mesh);
  // Read meshing dictionary
  const word dictName{"snappyHexMeshDict"};
  #include "set_system_mesh_dictionary_io.inc"
  const IOdictionary meshDict{dictIO};
  // all surface geometry
  const dictionary& geometryDict = meshDict.subDict("geometry");
  // refinement parameters
  const dictionary& refineDict = meshDict.subDict("castellatedMeshControls");
  // mesh motion and mesh quality parameters
  const dictionary& motionDict = meshDict.subDict("meshQualityControls");
  // snap-to-surface parameters
  const dictionary& snapDict = meshDict.subDict("snapControls");
  // layer addition parameters
  const dictionary& layerDict = meshDict.subDict("addLayersControls");
  // absolute merge distance
  const scalar mergeDist =
    getMergeDistance(mesh, readScalar(meshDict.lookup("mergeTolerance")));
  // Read decomposePar dictionary
  dictionary decomposeDict;

  {
    if (Pstream::parRun()) {
      decomposeDict = IOdictionary
      {
        // IOobject
        {
          "decomposeParDict",
          runTime.system(),
          mesh,
          IOobject::MUST_READ_IF_MODIFIED,
          IOobject::NO_WRITE
        }
      };
    } else {
      decomposeDict.add("method", "none");
      decomposeDict.add("numberOfSubdomains", 1);
    }
  }
  // Debug
  // ~~~~~
  // Set debug level
  meshRefinement::debugType debugLevel =
    meshRefinement::debugType
    (
      meshDict.lookupOrDefault<label>
      (
        "debug",
        0
      )
    );
  {
    wordList flags;
    if (meshDict.readIfPresent("debugFlags", flags)) {
      debugLevel =
        meshRefinement::debugType
        (
          meshRefinement::readFlags
          (
            meshRefinement::IOdebugTypeNames,
            flags
          )
        );
    }
  }
  if (debugLevel > 0) {
    meshRefinement::debug   = debugLevel;
    autoRefineDriver::debug = debugLevel;
    autoSnapDriver::debug   = debugLevel;
    autoLayerDriver::debug  = debugLevel;
  }
  // Set file writing level
  {
    wordList flags;
    if (meshDict.readIfPresent("writeFlags", flags)) {
      meshRefinement::writeLevel
      (
        meshRefinement::writeType
        (
          meshRefinement::readFlags
          (
            meshRefinement::IOwriteTypeNames,
            flags
          )
        )
      );
    }
  }
  // Set output level
  {
    wordList flags;
    if (meshDict.readIfPresent("outputFlags", flags)) {
      meshRefinement::outputLevel
      (
        meshRefinement::outputType
        (
          meshRefinement::readFlags
          (
            meshRefinement::IOoutputTypeNames,
            flags
          )
        )
      );
    }
  }
  // Read geometry
  searchableSurfaces allGeometry
  {
    // IOobject
    {
      "abc",                      // dummy name
      mesh.time().constant(),     // instance
      //mesh.time().findInstance("triSurface", word::null),// instance
      "triSurface",               // local
      mesh.time(),                // registry
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    geometryDict,
    meshDict.lookupOrDefault("singleRegionName", true)
  };
  // Read refinement surfaces
  autoPtr<refinementSurfaces> surfacesPtr;
  Info << "Reading refinement surfaces." << endl;
  if (surfaceSimplify) {
    IOdictionary foamyHexMeshDict
    {
      // IOobject
      {
        "foamyHexMeshDict",
        runTime.system(),
        runTime,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    const dictionary& conformationDict =
      foamyHexMeshDict
      .subDict("surfaceConformation")
      .subDict("geometryToConformTo");
    const dictionary& motionDict =
      foamyHexMeshDict.subDict("motionControl");
    const dictionary& shapeControlDict =
      motionDict.subDict("shapeControlFunctions");
    // Calculate current ratio of hex cells v.s. wanted cell size
    const scalar defaultCellSize =
      readScalar(motionDict.lookup("defaultCellSize"));
    const scalar initialCellSize = ::pow(meshPtr().V()[0], 1.0/3.0);
    surfacesPtr =
      createRefinementSurfaces
      (
        allGeometry,
        conformationDict,
        shapeControlDict,
        refineDict.lookupOrDefault("gapLevelIncrement", 0),
        initialCellSize/defaultCellSize
      );
  } else {
    surfacesPtr.set
    (
      new refinementSurfaces
      {
        allGeometry,
        refineDict.subDict("refinementSurfaces"),
        refineDict.lookupOrDefault("gapLevelIncrement", 0)
      }
    );
    Info<< "Read refinement surfaces in = "
      << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  }
  refinementSurfaces& surfaces = surfacesPtr();
  // Checking only?
  if (checkGeometry) {
    // Extract patchInfo
    List<wordList> patchTypes{allGeometry.size()};
    const PtrList<dictionary>& patchInfo = surfaces.patchInfo();
    const labelList& surfaceGeometry = surfaces.surfaces();
    FOR_ALL(surfaceGeometry, surfI) {
      label geomI = surfaceGeometry[surfI];
      const wordList& regNames = allGeometry.regionNames()[geomI];
      patchTypes[geomI].setSize(regNames.size());
      FOR_ALL(regNames, regionI) {
        label globalRegionI = surfaces.globalRegion(surfI, regionI);
        if (patchInfo.set(globalRegionI)) {
          patchTypes[geomI][regionI] =
            word(patchInfo[globalRegionI].lookup("type"));
        } else {
          patchTypes[geomI][regionI] = wallPolyPatch::typeName;
        }
      }
    }
    // Write some stats
    allGeometry.writeStats(patchTypes, Info);
    // Check topology
    allGeometry.checkTopology(true);
    // Check geometry
    allGeometry.checkGeometry
    (
      100.0,      // max size ratio
      1e-9,       // intersection tolerance
      autoPtr<writer<scalar>>(new vtkSetWriter<scalar>()),
      0.01,       // min triangle quality
      true
    );
    return 0;
  }
  // Read refinement shells
  Info << "Reading refinement shells." << endl;
  shellSurfaces shells
  {
    allGeometry,
    refineDict.subDict("refinementRegions")
  };
  Info << "Read refinement shells in = "
    << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  Info << "Setting refinement level of surface to be consistent"
    << " with shells." << endl;
  surfaces.setMinLevelFields(shells);
  Info << "Checked shell refinement in = "
    << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  // Read feature meshes
  Info << "Reading features." << endl;
  refinementFeatures features
  {
    mesh,
    refineDict.lookup("features")
  };
  Info << "Read features in = "
    << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  // Refinement engine
  Info << nl
    << "Determining initial surface intersections" << nl
    << "-----------------------------------------" << nl
    << endl;
  // Main refinement engine
  meshRefinement meshRefiner
  {
    mesh,
    mergeDist,          // tolerance used in sorting coordinates
    overwrite,          // overwrite mesh files?
    surfaces,           // for surface intersection refinement
    features,           // for feature edges/point based refinement
    shells              // for volume (inside/outside) refinement
  };
  Info << "Calculated surface intersections in = "
    << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  // Some stats
  meshRefiner.printMeshInfo(debugLevel, "Initial mesh");
  meshRefiner.write
  (
    meshRefinement::debugType(debugLevel&meshRefinement::OBJINTERSECTIONS),
    meshRefinement::writeType(0),
    mesh.time().path()/meshRefiner.timeName()
  );
  // Add all the surface regions as patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //- Global surface region to patch (non faceZone surface) or patches
  //  (faceZone surfaces)
  labelList globalToMasterPatch;
  labelList globalToSlavePatch;

  {
    Info << nl
      << "Adding patches for surface regions" << nl
      << "----------------------------------" << nl
      << endl;
    // From global region number to mesh patch.
    globalToMasterPatch.setSize(surfaces.nRegions(), -1);
    globalToSlavePatch.setSize(surfaces.nRegions(), -1);
    Info << setf(ios_base::left)
      << setw(6) << "Patch"
      << setw(20) << "Type"
      << setw(30) << "Region" << nl
      << setw(6) << "-----"
      << setw(20) << "----"
      << setw(30) << "------" << endl;
    const labelList& surfaceGeometry = surfaces.surfaces();
    const PtrList<dictionary>& surfacePatchInfo = surfaces.patchInfo();
    FOR_ALL(surfaceGeometry, surfI) {
      label geomI = surfaceGeometry[surfI];
      const wordList& regNames = allGeometry.regionNames()[geomI];
      Info << surfaces.names()[surfI] << ':' << nl << nl;
      if (surfaces.surfZones()[surfI].faceZoneName().empty()) {
        // 'Normal' surface
        FOR_ALL(regNames, i) {
          label globalRegionI = surfaces.globalRegion(surfI, i);
          label patchI;
          if (surfacePatchInfo.set(globalRegionI)) {
            patchI = meshRefiner.addMeshedPatch
            (
              regNames[i],
              surfacePatchInfo[globalRegionI]
            );
          } else {
            dictionary patchInfo;
            patchInfo.set("type", wallPolyPatch::typeName);
            patchI =
              meshRefiner.addMeshedPatch
              (
                regNames[i],
                patchInfo
              );
          }
          Info << setf(ios_base::left)
            << setw(6) << patchI
            << setw(20) << mesh.boundaryMesh()[patchI].type()
            << setw(30) << regNames[i] << nl;
          globalToMasterPatch[globalRegionI] = patchI;
          globalToSlavePatch[globalRegionI] = patchI;
        }
      } else {
        // Zoned surface
        FOR_ALL(regNames, i) {
          label globalRegionI = surfaces.globalRegion(surfI, i);
          // Add master side patch
          {
            label patchI;
            if (surfacePatchInfo.set(globalRegionI)) {
              patchI = meshRefiner.addMeshedPatch
              (
                regNames[i],
                surfacePatchInfo[globalRegionI]
              );
            } else {
              dictionary patchInfo;
              patchInfo.set("type", wallPolyPatch::typeName);
              patchI =
                meshRefiner.addMeshedPatch
                (
                  regNames[i],
                  patchInfo
                );
            }
            Info << setf(ios_base::left)
              << setw(6) << patchI
              << setw(20) << mesh.boundaryMesh()[patchI].type()
              << setw(30) << regNames[i] << nl;
            globalToMasterPatch[globalRegionI] = patchI;
          }
          // Add slave side patch
          {
            const word slaveName = regNames[i] + "_slave";
            label patchI;
            if (surfacePatchInfo.set(globalRegionI)) {
              patchI =
                meshRefiner.addMeshedPatch
                (
                  slaveName,
                  surfacePatchInfo[globalRegionI]
                );
            } else {
              dictionary patchInfo;
              patchInfo.set("type", wallPolyPatch::typeName);
              patchI =
                meshRefiner.addMeshedPatch
                (
                  slaveName,
                  patchInfo
                );
            }
            Info << setf(ios_base::left)
              << setw(6) << patchI
              << setw(20) << mesh.boundaryMesh()[patchI].type()
              << setw(30) << slaveName << nl;
            globalToSlavePatch[globalRegionI] = patchI;
          }
        }
      }
      Info << nl;
    }
    Info << "Added patches in = "
      << mesh.time().cpuTimeIncrement() << " s" << nl << endl;
  }
  // Parallel
  // ~~~~~~~~
  // Decomposition
  autoPtr<decompositionMethod> decomposerPtr
  {
    decompositionMethod::New
    (
      decomposeDict
    )
  };
  decompositionMethod& decomposer = decomposerPtr();
  if (Pstream::parRun() && !decomposer.parallelAware()) {
    FATAL_ERROR_IN(args.executable())
      << "You have selected decomposition method "
      << decomposer.typeName
      << " which is not parallel aware." << endl
      << "Please select one that is (hierarchical, ptscotch)"
      << exit(FatalError);
  }
  // Mesh distribution engine (uses tolerance to reconstruct meshes)
  fvMeshDistribute distributor{mesh, mergeDist};
  // Now do the real work -refinement -snapping -layers
  const Switch wantRefine{meshDict.lookup("castellatedMesh")};
  const Switch wantSnap{meshDict.lookup("snap")};
  const Switch wantLayers{meshDict.lookup("addLayers")};
  // Refinement parameters
  const refinementParameters refineParams{refineDict};
  // Snap parameters
  const snapParameters snapParams{snapDict};
  // Layer addition parameters
  const layerParameters layerParams{layerDict, mesh.boundaryMesh()};
  if (wantRefine) {
    cpuTime timer;
    autoRefineDriver refineDriver
    {
      meshRefiner,
      decomposer,
      distributor,
      globalToMasterPatch,
      globalToSlavePatch
    };
    if (!overwrite && !debugLevel) {
      const_cast<Time&>(mesh.time())++;
    }
    refineDriver.doRefine
    (
      refineDict,
      refineParams,
      snapParams,
      refineParams.handleSnapProblems(),
      motionDict
    );
    writeMesh
    (
      "Refined mesh",
      meshRefiner,
      debugLevel,
      meshRefinement::writeLevel()
    );
    Info << "Mesh refined in = "
      << timer.cpuTimeIncrement() << " s." << endl;
  }
  if (wantSnap) {
    cpuTime timer;
    autoSnapDriver snapDriver
    {
      meshRefiner,
      globalToMasterPatch,
      globalToSlavePatch
    };
    if (!overwrite && !debugLevel) {
      const_cast<Time&>(mesh.time())++;
    }
    // Use the resolveFeatureAngle from the refinement parameters
    scalar curvature = refineParams.curvature();
    scalar planarAngle = refineParams.planarAngle();
    snapDriver.doSnap
    (
      snapDict,
      motionDict,
      curvature,
      planarAngle,
      snapParams
    );
    writeMesh
    (
      "Snapped mesh",
      meshRefiner,
      debugLevel,
      meshRefinement::writeLevel()
    );
    Info << "Mesh snapped in = "
      << timer.cpuTimeIncrement() << " s." << endl;
  }
  if (wantLayers) {
    cpuTime timer;
    autoLayerDriver layerDriver
    {
      meshRefiner,
      globalToMasterPatch,
      globalToSlavePatch
    };
    // Use the maxLocalCells from the refinement parameters
    bool preBalance = returnReduce
    (
      (mesh.nCells() >= refineParams.maxLocalCells()),
      orOp<bool>()
    );
    if (!overwrite && !debugLevel) {
      const_cast<Time&>(mesh.time())++;
    }
    layerDriver.doLayers
    (
      layerDict,
      motionDict,
      layerParams,
      preBalance,
      decomposer,
      distributor
    );
    writeMesh
    (
      "Layer mesh",
      meshRefiner,
      debugLevel,
      meshRefinement::writeLevel()
    );
    Info << "Layers added in = "
      << timer.cpuTimeIncrement() << " s." << endl;
  }
  {
    // Check final mesh
    Info << "Checking final mesh ..." << endl;
    faceSet wrongFaces{mesh, "wrongFaces", mesh.nFaces()/100};
    motionSmoother::checkMesh(false, mesh, motionDict, wrongFaces);
    const label nErrors =
      returnReduce
      (
        wrongFaces.size(),
        sumOp<label>()
      );
    if (nErrors > 0) {
      Info << "Finished meshing with " << nErrors << " illegal faces"
        << " (concave, zero area or negative cell pyramid volume)"
        << endl;
      wrongFaces.write();
    } else {
      Info << "Finished meshing without any errors" << endl;
    }
  }
  if (surfaceSimplify) {
    const polyBoundaryMesh& bMesh = mesh.boundaryMesh();
    labelHashSet includePatches{bMesh.size()};
    if (args.optionFound("patches")) {
      includePatches =
        bMesh.patchSet
        (
          wordReList{args.optionLookup("patches")()}
        );
    } else {
      FOR_ALL(bMesh, patchI) {
        const polyPatch& patch = bMesh[patchI];
        if (!isA<processorPolyPatch>(patch)) {
          includePatches.insert(patchI);
        }
      }
    }
    fileName outFileName
    {
      args.optionLookupOrDefault<fileName>
      (
        "outFile",
        "constant/triSurface/simplifiedSurface.stl"
      )
    };
    extractSurface
    (
      mesh,
      runTime,
      includePatches,
      outFileName
    );
    pointIOField cellCentres
    {
      // IOobject
      {
        "internalCellCentres",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      mesh.cellCentres()
    };
    cellCentres.write();
  }
  Info << "Finished meshing in = "
    << runTime.elapsedCpuTime() << " s." << endl;
  Info << "End\n" << endl;
  return 0;
}

