// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "basic_kinematic_colliding_cloud.hpp"


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
  #include "create_dynamic_fv_mesh.inc"
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    mesh.update();
    U.correctBoundaryConditions();
    Info << "Evolving " << kinematicCloud.name() << endl;
    laminarTransport.correct();
    mu = laminarTransport.nu()*rhoInfValue;
    kinematicCloud.evolve();
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

