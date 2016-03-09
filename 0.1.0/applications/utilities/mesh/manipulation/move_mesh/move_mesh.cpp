// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "motion_solver.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  autoPtr<motionSolver> motionPtr = motionSolver::New(mesh);
  while (runTime.loop())
  {
    Info << "Time = " << runTime.timeName() << endl;
    mesh.movePoints(motionPtr->newPoints());
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
