// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "channel_index.hpp"
#include "make_graph.hpp"
#include "os_specific.hpp"

int main(int argc, char *argv[])
{
  argList::noParallel();
  timeSelector::addOptions();
  #include "set_root_case.inc"
  #include "create_time.inc"
  // Get times list
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_mesh.inc"
  #include "read_transport_properties.inc"
  const word& gFormat = runTime.graphFormat();
  // Setup channel indexing for averaging over channel down to a line
  IOdictionary channelDict
  {
    {
      "postChannelDict",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  channelIndex channelIndexing{mesh, channelDict};
  // For each time step read all fields
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Collapsing fields for time " << runTime.timeName() << endl;
    #include "read_fields.inc"
    #include "calculate_fields.inc"
    // Average fields over channel down to a line
    #include "collapse.inc"
  }
  Info << "\nEnd\n" << endl;
  return 0;
}
