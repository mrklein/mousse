// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time.hpp"
#include "iodictionary.hpp"
#include "ioptr_list.hpp"
#include "block_mesh.hpp"
#include "attach_poly_topo_changer.hpp"
#include "empty_poly_patch.hpp"
#include "cell_set.hpp"
#include "arg_list.hpp"
#include "os_specific.hpp"
#include "ofstream.hpp"
#include "pair.hpp"
#include "sliding_interface.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  argList::noParallel();
  argList::addBoolOption
  (
    "blockTopology",
    "write block edges and centres as .obj files"
  );
  argList::addOption
  (
    "dict",
    "file",
    "specify alternative dictionary for the blockMesh description"
  );
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  const word dictName{"blockMeshDict"};
  word regionName;
  word regionPath;
  // Check if the region is specified otherwise mesh the default region
  if (args.optionReadIfPresent("region", regionName, polyMesh::defaultRegion))
  {
    Info<< nl << "Generating mesh for region " << regionName << endl;
    regionPath = regionName;
  }
  // Search for the appropriate blockMesh dictionary....
  fileName dictPath;
  // Check if the dictionary is specified on the command-line
  if (args.optionFound("dict"))
  {
    dictPath = args["dict"];
    dictPath =
    (
      isDir(dictPath)
     ? dictPath/dictName
     : dictPath
    );
  }
  // Check if dictionary is present in the constant directory
  else if
  (
    exists(runTime.path()/runTime.constant()
           /regionPath/polyMesh::meshSubDir/dictName)
  )
  {
    dictPath = runTime.constant()/regionPath/polyMesh::meshSubDir/dictName;
  }
  // Otherwise assume the dictionary is present in the system directory
  else
  {
    dictPath = runTime.system()/regionPath/dictName;
  }
  IOobject meshDictIO
  {
    dictPath,
    runTime,
    IOobject::MUST_READ,
    IOobject::NO_WRITE,
    false
  };
  if (!meshDictIO.headerOk())
  {
    FATAL_ERROR_IN(args.executable())
      << "Cannot open mesh description file\n    "
      << meshDictIO.objectPath()
      << nl
      << exit(FatalError);
  }
  Info<< "Creating block mesh from\n    "
    << meshDictIO.objectPath() << endl;
  blockMesh::verbose(true);
  IOdictionary meshDict{meshDictIO};
  blockMesh blocks{meshDict, regionName};
  if (args.optionFound("blockTopology"))
  {

    // Write mesh as edges.
    {
      fileName objMeshFile{"blockTopology.obj"};
      OFstream str{runTime.path()/objMeshFile};
      Info<< nl << "Dumping block structure as Lightwave obj format"
        << " to " << objMeshFile << endl;
      blocks.writeTopology(str);
    }

    // Write centres of blocks
    {
      fileName objCcFile{"blockCentres.obj"};
      OFstream str{runTime.path()/objCcFile};
      Info<< nl << "Dumping block centres as Lightwave obj format"
        << " to " << objCcFile << endl;
      const polyMesh& topo = blocks.topology();
      const pointField& cellCentres = topo.cellCentres();
      FOR_ALL(cellCentres, cellI)
      {
        //point cc = b.blockShape().centre(b.points());
        const point& cc = cellCentres[cellI];
        str << "v " << cc.x() << ' ' << cc.y() << ' ' << cc.z() << nl;
      }
    }

    Info<< nl << "end" << endl;
    return 0;
  }
  Info<< nl << "Creating polyMesh from blockMesh" << endl;
  word defaultFacesName = "defaultFaces";
  word defaultFacesType = emptyPolyPatch::typeName;
  polyMesh mesh
  {
    // IOobject
    {
      regionName,
      runTime.constant(),
      runTime
    },
    xferCopy(blocks.points()),           // could we re-use space?
    blocks.cells(),
    blocks.patches(),
    blocks.patchNames(),
    blocks.patchDicts(),
    defaultFacesName,
    defaultFacesType
  };
  // Read in a list of dictionaries for the merge patch pairs
  if (meshDict.found("mergePatchPairs"))
  {
    List<Pair<word>> mergePatchPairs{meshDict.lookup("mergePatchPairs")};
    #include "merge_patch_pairs.inc"
  }
  else
  {
    Info<< nl << "There are no merge patch pairs edges" << endl;
  }
  // Set any cellZones (note: cell labelling unaffected by above
  // mergePatchPairs)
  label nZones = blocks.numZonedBlocks();
  if (nZones > 0)
  {
    Info<< nl << "Adding cell zones" << endl;
    // Map from zoneName to cellZone index
    HashTable<label> zoneMap{nZones};
    // Cells per zone.
    List<DynamicList<label>> zoneCells{nZones};
    // Running cell counter
    label cellI = 0;
    // Largest zone so far
    label freeZoneI = 0;
    FOR_ALL(blocks, blockI)
    {
      const block& b = blocks[blockI];
      const labelListList& blockCells = b.cells();
      const word& zoneName = b.blockDef().zoneName();
      if (zoneName.size())
      {
        HashTable<label>::const_iterator iter = zoneMap.find(zoneName);
        label zoneI;
        if (iter == zoneMap.end())
        {
          zoneI = freeZoneI++;
          Info<< "    " << zoneI << '\t' << zoneName << endl;
          zoneMap.insert(zoneName, zoneI);
        }
        else
        {
          zoneI = iter();
        }
        FOR_ALL(blockCells, i)
        {
          zoneCells[zoneI].append(cellI++);
        }
      }
      else
      {
        cellI += b.cells().size();
      }
    }
    List<cellZone*> cz{zoneMap.size()};
    Info<< nl << "Writing cell zones as cellSets" << endl;
    FOR_ALL_CONST_ITER(HashTable<label>, zoneMap, iter)
    {
      label zoneI = iter();
      cz[zoneI] = new cellZone
      {
        iter.key(),
        zoneCells[zoneI].shrink(),
        zoneI,
        mesh.cellZones()
      };
      // Write as cellSet for ease of processing
      cellSet cset(mesh, iter.key(), zoneCells[zoneI].shrink());
      cset.write();
    }
    mesh.pointZones().setSize(0);
    mesh.faceZones().setSize(0);
    mesh.cellZones().setSize(0);
    mesh.addZones(List<pointZone*>(0), List<faceZone*>(0), cz);
  }
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  Info<< nl << "Writing polyMesh" << endl;
  mesh.removeFiles();
  if (!mesh.write())
  {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing polyMesh."
      << exit(FatalError);
  }
  //
  // write some information
  //
  {
    const polyPatchList& patches = mesh.boundaryMesh();
    Info<< "----------------" << nl
      << "Mesh Information" << nl
      << "----------------" << nl
      << "  " << "boundingBox: " << boundBox(mesh.points()) << nl
      << "  " << "nPoints: " << mesh.nPoints() << nl
      << "  " << "nCells: " << mesh.nCells() << nl
      << "  " << "nFaces: " << mesh.nFaces() << nl
      << "  " << "nInternalFaces: " << mesh.nInternalFaces() << nl;
    Info<< "----------------" << nl
      << "Patches" << nl
      << "----------------" << nl;
    for (const auto& p : patches)
    {
      Info<< "  " << "patch " << p.index()
        << " (start: " << p.start()
        << " size: " << p.size()
        << ") name: " << p.name()
        << nl;
    }
  }
  Info<< "\nEnd\n" << endl;
  return 0;
}
