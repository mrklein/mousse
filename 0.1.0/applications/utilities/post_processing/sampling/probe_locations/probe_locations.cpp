// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "io_probes.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  IOprobes sniff
  {
    probes::typeName,
    mesh,
    word("probesDict"), // force the use of the system directory
    IOobject::MUST_READ,
    true
  };
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    // Handle geometry/topology changes
    polyMesh::readUpdateState state = mesh.readUpdate();
    if (state == polyMesh::POINTS_MOVED || state == polyMesh::TOPO_CHANGE)
    {
      sniff.read();
    }
    sniff.write();
    Info << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
