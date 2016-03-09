// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "engine_time.hpp"
#include "engine_mesh.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_engine_time.inc"
  #include "create_engine_mesh.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop())
  {
    Info << "Time = " << runTime.theta() << " CA-deg\n" << endl;
    mesh.move();
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
