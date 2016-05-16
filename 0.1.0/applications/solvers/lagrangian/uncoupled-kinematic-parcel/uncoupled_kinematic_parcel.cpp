// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "psi_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "basic_kinematic_cloud.hpp"


int main(int argc, char *argv[])
{
  argList::addOption
  (
    "cloudName",
    "name",
    "specify alternative cloud name. default is 'kinematicCloud'"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "compressible_courant_no.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    Info << "Evolving " << kinematicCloud.name() << endl;
    kinematicCloud.evolve();
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
