// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "istring_stream.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"
#include "ofstream.hpp"
#include "ifstream.hpp"
#include "ioobject_list.hpp"
#include "sortable_list.hpp"
#include "time_selector.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  timeSelector::addOptions(true, false);
  argList::addNote
  (
    "add point/face/cell Zones from similar named point/face/cell Sets"
  );
  argList::addBoolOption
  (
    "noFlipMap",
    "ignore orientation of faceSet"
  );
  #include "add_region_option.inc"
  #include "add_time_options.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  const bool noFlipMap = args.optionFound("noFlipMap");
  // Get times list
  (void)timeSelector::selectIfPresent(runTime, args);
  #include "create_named_poly_mesh.inc"
  // Search for list of objects for the time of the mesh
  word setsInstance = runTime.findInstance
  (
    polyMesh::meshSubDir/"sets",
    word::null,
    IOobject::MUST_READ,
    mesh.facesInstance()
  );
  IOobjectList objects{mesh, setsInstance, polyMesh::meshSubDir/"sets"};
  Info << "Searched : " << setsInstance/polyMesh::meshSubDir/"sets"
    << nl
    << "Found    : " << objects.names() << nl
    << endl;
  IOobjectList pointObjects{objects.lookupClass(pointSet::typeName)};
  FOR_ALL_CONST_ITER(IOobjectList, pointObjects, iter)
  {
    // Not in memory. Load it.
    pointSet set{*iter()};
    SortableList<label> pointLabels{set.toc()};
    label zoneID = mesh.pointZones().findZoneID(set.name());
    if (zoneID == -1)
    {
      Info << "Adding set " << set.name() << " as a pointZone." << endl;
      label sz = mesh.pointZones().size();
      mesh.pointZones().setSize(sz+1);
      mesh.pointZones().set
      (
        sz,
        new pointZone
        {
          set.name(),             //name
          pointLabels,            //addressing
          sz,                     //index
          mesh.pointZones()       //pointZoneMesh
        }
      );
      mesh.pointZones().writeOpt() = IOobject::AUTO_WRITE;
      mesh.pointZones().instance() = mesh.facesInstance();
    }
    else
    {
      Info << "Overwriting contents of existing pointZone " << zoneID
        << " with that of set " << set.name() << "." << endl;
      mesh.pointZones()[zoneID] = pointLabels;
      mesh.pointZones().writeOpt() = IOobject::AUTO_WRITE;
      mesh.pointZones().instance() = mesh.facesInstance();
    }
  }
  IOobjectList faceObjects{objects.lookupClass(faceSet::typeName)};
  HashSet<word> slaveCellSets;
  FOR_ALL_CONST_ITER(IOobjectList, faceObjects, iter)
  {
    // Not in memory. Load it.
    faceSet set{*iter()};
    SortableList<label> faceLabels{set.toc()};
    DynamicList<label> addressing{set.size()};
    DynamicList<bool> flipMap{set.size()};
    if (noFlipMap)
    {
      // No flip map.
      FOR_ALL(faceLabels, i)
      {
        label faceI = faceLabels[i];
        addressing.append(faceI);
        flipMap.append(false);
      }
    }
    else
    {
      const word setName{set.name() + "SlaveCells"};
      Info << "Trying to load cellSet " << setName
        << " to find out the slave side of the zone." << nl
        << "If you do not care about the flipMap"
        << " (i.e. do not use the sideness)" << nl
        << "use the -noFlipMap command line option."
        << endl;
      // Load corresponding cells
      cellSet cells{mesh, setName};
      // Store setName to exclude from cellZones further on
      slaveCellSets.insert(setName);
      FOR_ALL(faceLabels, i)
      {
        label faceI = faceLabels[i];
        bool flip = false;
        if (mesh.isInternalFace(faceI))
        {
          if (cells.found(mesh.faceOwner()[faceI])
              && !cells.found(mesh.faceNeighbour()[faceI]))
          {
            flip = false;
          }
          else if (!cells.found(mesh.faceOwner()[faceI])
                   && cells.found(mesh.faceNeighbour()[faceI]))
          {
            flip = true;
          }
          else
          {
            FATAL_ERROR_IN(args.executable())
              << "One of owner or neighbour of internal face "
              << faceI << " should be in cellSet " << cells.name()
              << " to be able to determine orientation." << endl
              << "Face:" << faceI
              << " own:" << mesh.faceOwner()[faceI]
              << " OwnInCellSet:"
              << cells.found(mesh.faceOwner()[faceI])
              << " nei:" << mesh.faceNeighbour()[faceI]
              << " NeiInCellSet:"
              << cells.found(mesh.faceNeighbour()[faceI])
              << abort(FatalError);
          }
        }
        else
        {
          if (cells.found(mesh.faceOwner()[faceI]))
          {
            flip = false;
          }
          else
          {
            flip = true;
          }
        }
        addressing.append(faceI);
        flipMap.append(flip);
      }
    }
    label zoneID = mesh.faceZones().findZoneID(set.name());
    if (zoneID == -1)
    {
      Info << "Adding set " << set.name() << " as a faceZone." << endl;
      label sz = mesh.faceZones().size();
      mesh.faceZones().setSize(sz+1);
      mesh.faceZones().set
      (
        sz,
        new faceZone
        {
          set.name(),             //name
          addressing.shrink(),    //addressing
          flipMap.shrink(),       //flipmap
          sz,                     //index
          mesh.faceZones()        //pointZoneMesh
        }
      );
      mesh.faceZones().writeOpt() = IOobject::AUTO_WRITE;
      mesh.faceZones().instance() = mesh.facesInstance();
    }
    else
    {
      Info << "Overwriting contents of existing faceZone " << zoneID
        << " with that of set " << set.name() << "." << endl;
      mesh.faceZones()[zoneID].resetAddressing
      (
        addressing.shrink(),
        flipMap.shrink()
      );
      mesh.faceZones().writeOpt() = IOobject::AUTO_WRITE;
      mesh.faceZones().instance() = mesh.facesInstance();
    }
  }
  IOobjectList cellObjects{objects.lookupClass(cellSet::typeName)};
  FOR_ALL_CONST_ITER(IOobjectList, cellObjects, iter)
  {
    if (!slaveCellSets.found(iter.key()))
    {
      // Not in memory. Load it.
      cellSet set{*iter()};
      SortableList<label> cellLabels{set.toc()};
      label zoneID = mesh.cellZones().findZoneID(set.name());
      if (zoneID == -1)
      {
        Info << "Adding set " << set.name() << " as a cellZone." << endl;
        label sz = mesh.cellZones().size();
        mesh.cellZones().setSize(sz+1);
        mesh.cellZones().set
        (
          sz,
          new cellZone
          {
            set.name(),             //name
            cellLabels,             //addressing
            sz,                     //index
            mesh.cellZones()        //pointZoneMesh
          }
        );
        mesh.cellZones().writeOpt() = IOobject::AUTO_WRITE;
        mesh.cellZones().instance() = mesh.facesInstance();
      }
      else
      {
        Info << "Overwriting contents of existing cellZone " << zoneID
          << " with that of set " << set.name() << "." << endl;
        mesh.cellZones()[zoneID] = cellLabels;
        mesh.cellZones().writeOpt() = IOobject::AUTO_WRITE;
        mesh.cellZones().instance() = mesh.facesInstance();
      }
    }
  }
  Info << "Writing mesh." << endl;
  if (!mesh.write())
  {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing polyMesh."
      << exit(FatalError);
  }
  Info << "\nEnd\n" << endl;
  return 0;
}
