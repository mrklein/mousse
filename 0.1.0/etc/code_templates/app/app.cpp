// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
int main(int argc, char *argv[])
{
  #include "set_root_case.hpp"
  #include "create_time.hpp"
    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
    << "  ClockTime = " << runTime.elapsedClockTime() << " s"
    << nl << endl;
  Info<< "End\n" << endl;
  return 0;
}
