// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dsmc_cloud.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"

  Info<< nl << "Constructing dsmcCloud " << endl;
  dsmcCloud dsmc{"dsmc", mesh};
  Info<< "\nStarting time loop\n" << endl;
  while (runTime.loop())
  {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    dsmc.evolve();
    dsmc.info();
    runTime.write();
    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
