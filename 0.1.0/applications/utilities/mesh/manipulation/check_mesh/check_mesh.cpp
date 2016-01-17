// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "global_mesh_data.hpp"
#include "print_mesh_stats.hpp"
#include "check_topology.hpp"
#include "check_geometry.hpp"
#include "check_mesh_quality.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  argList::addBoolOption
  (
    "noTopology",
    "skip checking the mesh topology"
  );
  argList::addBoolOption
  (
    "allGeometry",
    "include bounding box checks"
  );
  argList::addBoolOption
  (
    "allTopology",
    "include extra topology checks"
  );
  argList::addBoolOption
  (
    "meshQuality",
    "read user-defined mesh quality criterions from system/meshQualityDict"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_poly_mesh.inc"
  const bool noTopology  = args.optionFound("noTopology");
  const bool allGeometry = args.optionFound("allGeometry");
  const bool allTopology = args.optionFound("allTopology");
  const bool meshQuality = args.optionFound("meshQuality");
  if (noTopology)
  {
    Info<< "Disabling all topology checks." << nl << endl;
  }
  if (allTopology)
  {
    Info<< "Enabling all (cell, face, edge, point) topology checks."
      << nl << endl;
  }
  if (allGeometry)
  {
    Info<< "Enabling all geometry checks." << nl << endl;
  }
  if (meshQuality)
  {
    Info<< "Enabling user-defined geometry checks." << nl << endl;
  }
  autoPtr<IOdictionary> qualDict;
  if (meshQuality)
  {
    qualDict.reset
    (
      new IOdictionary
      {
        // IOobject
        {
          "meshQualityDict",
          mesh.time().system(),
          mesh,
          IOobject::MUST_READ,
        }
      }
    );
  }
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    polyMesh::readUpdateState state = mesh.readUpdate();
    if (!timeI || state == polyMesh::TOPO_CHANGE
        || state == polyMesh::TOPO_PATCH_CHANGE)
    {
      Info<< "Time = " << runTime.timeName() << nl << endl;
      // Clear mesh before checking
      mesh.clearOut();
      // Reconstruct globalMeshData
      mesh.globalData();
      printMeshStats(mesh, allTopology);
      label nFailedChecks = 0;
      if (!noTopology)
      {
        nFailedChecks += checkTopology(mesh, allTopology, allGeometry);
      }
      nFailedChecks += checkGeometry(mesh, allGeometry);
      if (meshQuality)
      {
        nFailedChecks += checkMeshQuality(mesh, qualDict());
      }
      // Note: no reduction in nFailedChecks necessary since is
      //       counter of checks, not counter of failed cells,faces etc.
      if (nFailedChecks == 0)
      {
        Info<< "\nMesh OK.\n" << endl;
      }
      else
      {
        Info<< "\nFailed " << nFailedChecks << " mesh checks.\n"
          << endl;
      }
    }
    else if (state == polyMesh::POINTS_MOVED)
    {
      Info<< "Time = " << runTime.timeName() << nl << endl;
      label nFailedChecks = checkGeometry(mesh, allGeometry);
      if (meshQuality)
      {
        nFailedChecks += checkMeshQuality(mesh, qualDict());
      }
      if (nFailedChecks)
      {
        Info<< "\nFailed " << nFailedChecks << " mesh checks.\n"
          << endl;
      }
      else
      {
        Info<< "\nMesh OK.\n" << endl;
      }
    }
  }
  Info<< "End\n" << endl;
  return 0;
}
