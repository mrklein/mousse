// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "topo_set_source.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"
#include "global_mesh_data.hpp"
#include "time_selector.hpp"
#include "ioobject_list.hpp"
#include "cell_zone_set.hpp"
#include "face_zone_set.hpp"
#include "point_zone_set.hpp"
using namespace mousse;
void printMesh(const Time& runTime, const polyMesh& mesh)
{
  Info<< "Time:" << runTime.timeName()
    << "  cells:" << mesh.globalData().nTotalCells()
    << "  faces:" << mesh.globalData().nTotalFaces()
    << "  points:" << mesh.globalData().nTotalPoints()
    << "  patches:" << mesh.boundaryMesh().size()
    << "  bb:" << mesh.bounds() << nl;
}
template<class ZoneType>
void removeZone
(
  ZoneMesh<ZoneType, polyMesh>& zones,
  const word& setName
)
{
  label zoneID = zones.findZoneID(setName);
  if (zoneID != -1)
  {
    Info<< "Removing zone " << setName << " at index " << zoneID << endl;
    // Shuffle to last position
    labelList oldToNew(zones.size());
    label newI = 0;
    forAll(oldToNew, i)
    {
      if (i != zoneID)
      {
        oldToNew[i] = newI++;
      }
    }
    oldToNew[zoneID] = newI;
    zones.reorder(oldToNew);
    // Remove last element
    zones.setSize(zones.size()-1);
    zones.clearAddressing();
    zones.write();
  }
}
// Physically remove a set
void removeSet
(
  const polyMesh& mesh,
  const word& setType,
  const word& setName
)
{
  // Remove the file
  IOobjectList objects
  (
    mesh,
    mesh.time().findInstance
    (
      polyMesh::meshSubDir/"sets",
      word::null,
      IOobject::READ_IF_PRESENT,
      mesh.facesInstance()
    ),
    polyMesh::meshSubDir/"sets"
  );
  if (objects.found(setName))
  {
    // Remove file
    fileName object = objects[setName]->objectPath();
    Info<< "Removing file " << object << endl;
    rm(object);
  }
  // See if zone
  if (setType == cellZoneSet::typeName)
  {
    removeZone
    (
      const_cast<cellZoneMesh&>(mesh.cellZones()),
      setName
    );
  }
  else if (setType == faceZoneSet::typeName)
  {
    removeZone
    (
      const_cast<faceZoneMesh&>(mesh.faceZones()),
      setName
    );
  }
  else if (setType == pointZoneSet::typeName)
  {
    removeZone
    (
      const_cast<pointZoneMesh&>(mesh.pointZones()),
      setName
    );
  }
}
polyMesh::readUpdateState meshReadUpdate(polyMesh& mesh)
{
  polyMesh::readUpdateState stat = mesh.readUpdate();
  switch(stat)
  {
    case polyMesh::UNCHANGED:
    {
      Info<< "    mesh not changed." << endl;
      break;
    }
    case polyMesh::POINTS_MOVED:
    {
      Info<< "    points moved; topology unchanged." << endl;
      break;
    }
    case polyMesh::TOPO_CHANGE:
    {
      Info<< "    topology changed; patches unchanged." << nl
        << "    ";
      printMesh(mesh.time(), mesh);
      break;
    }
    case polyMesh::TOPO_PATCH_CHANGE:
    {
      Info<< "    topology changed and patches changed." << nl
        << "    ";
      printMesh(mesh.time(), mesh);
      break;
    }
    default:
    {
      FatalErrorIn("meshReadUpdate(polyMesh&)")
        << "Illegal mesh update state "
        << stat  << abort(FatalError);
      break;
    }
  }
  return stat;
}
int main(int argc, char *argv[])
{
  timeSelector::addOptions(true, false);
  #include "add_dict_option.hpp"
  #include "add_region_option.hpp"
  argList::addBoolOption
  (
    "noSync",
    "do not synchronise selection across coupled patches"
  );
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  instantList timeDirs = timeSelector::selectIfPresent(runTime, args);
  #include "create_named_poly_mesh.hpp"
  const bool noSync = args.optionFound("noSync");
  const word dictName("topoSetDict");
  #include "set_system_mesh_dictionary_io.hpp"
  Info<< "Reading " << dictName << "\n" << endl;
  IOdictionary topoSetDict(dictIO);
  // Read set construct info from dictionary
  PtrList<dictionary> actions(topoSetDict.lookup("actions"));
  forAll(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    // Optionally re-read mesh
    meshReadUpdate(mesh);
    // Execute all actions
    forAll(actions, i)
    {
      const dictionary& dict = actions[i];
      const word setName(dict.lookup("name"));
      const word actionName(dict.lookup("action"));
      const word setType(dict.lookup("type"));
      topoSetSource::setAction action = topoSetSource::toAction
      (
        actionName
      );
      autoPtr<topoSet> currentSet;
      if
      (
        (action == topoSetSource::NEW)
      || (action == topoSetSource::CLEAR)
      )
      {
        currentSet = topoSet::New(setType, mesh, setName, 10000);
        Info<< "Created " << currentSet().type() << " "
          << setName << endl;
      }
      else if (action == topoSetSource::REMOVE)
      {
        //?
      }
      else
      {
        currentSet = topoSet::New
        (
          setType,
          mesh,
          setName,
          IOobject::MUST_READ
        );
        Info<< "Read set " << currentSet().type() << " "
          << setName << " with size "
          << returnReduce(currentSet().size(), sumOp<label>())
          << endl;
      }
      // Handle special actions (clear, invert) locally, rest through
      // sources.
      switch (action)
      {
        case topoSetSource::NEW:
        case topoSetSource::ADD:
        case topoSetSource::DELETE:
        {
          Info<< "    Applying source " << word(dict.lookup("source"))
            << endl;
          autoPtr<topoSetSource> source = topoSetSource::New
          (
            dict.lookup("source"),
            mesh,
            dict.subDict("sourceInfo")
          );
          source().applyToSet(action, currentSet());
          // Synchronize for coupled patches.
          if (!noSync) currentSet().sync(mesh);
          currentSet().write();
        }
        break;
        case topoSetSource::SUBSET:
        {
          Info<< "    Applying source " << word(dict.lookup("source"))
            << endl;
          autoPtr<topoSetSource> source = topoSetSource::New
          (
            dict.lookup("source"),
            mesh,
            dict.subDict("sourceInfo")
          );
          // Backup current set.
          autoPtr<topoSet> oldSet
          (
            topoSet::New
            (
              setType,
              mesh,
              currentSet().name() + "_old2",
              currentSet()
            )
          );
          currentSet().clear();
          source().applyToSet(topoSetSource::NEW, currentSet());
          // Combine new value of currentSet with old one.
          currentSet().subset(oldSet());
          // Synchronize for coupled patches.
          if (!noSync) currentSet().sync(mesh);
          currentSet().write();
        }
        break;
        case topoSetSource::CLEAR:
          Info<< "    Clearing " << currentSet().type() << endl;
          currentSet().clear();
          currentSet().write();
        break;
        case topoSetSource::INVERT:
          Info<< "    Inverting " << currentSet().type() << endl;
          currentSet().invert(currentSet().maxSize(mesh));
          currentSet().write();
        break;
        case topoSetSource::REMOVE:
          Info<< "    Removing set" << endl;
          removeSet(mesh, setType, setName);
        break;
        default:
          WarningIn(args.executable())
            << "Unhandled action " << action << endl;
        break;
      }
      if (currentSet.valid())
      {
        Info<< "    " << currentSet().type() << " "
          << currentSet().name()
          << " now size "
          << returnReduce(currentSet().size(), sumOp<label>())
          << endl;
      }
    }
  }
  Info<< "\nEnd\n" << endl;
  return 0;
}
