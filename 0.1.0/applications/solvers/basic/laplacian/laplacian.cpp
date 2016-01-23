// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "simple_control.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  simpleControl simple(mesh);
  #include "create_fields.inc"
    Info<< "\nCalculating temperature distribution\n" << endl;
  while (simple.loop())
  {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    while (simple.correctNonOrthogonal())
    {
      solve
      (
        fvm::ddt(T) - fvm::laplacian(DT, T)
      );
    }
    #include "write.inc"
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
