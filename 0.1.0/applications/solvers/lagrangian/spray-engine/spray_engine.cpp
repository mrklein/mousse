// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "engine_time.hpp"
#include "engine_mesh.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "basic_spray_cloud.hpp"
#include "psi_combustion_model.hpp"
#include "radiation_model.hpp"
#include "slg_thermo.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_engine_time.inc"
  #include "create_engine_mesh.inc"
  pimpleControl pimple{mesh};
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_rho_uf.inc"
  #include "create_clouds.inc"
  #include "create_radiation_model.inc"
  #include "init_continuity_errs.inc"
  #include "read_engine_time_controls.inc"
  #include "compressible_courant_no.inc"
  #include "set_initial_delta_t.inc"
  #include "start_summary.inc"

  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_engine_time_controls.inc"
    #include "compressible_courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info<< "Crank angle = " << runTime.theta() << " CA-deg" << endl;
    mesh.move();
    parcels.evolve();
    #include "rho_eqn.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "u_eqn.inc"
      #include "y_eqn.inc"
      #include "e_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct())
      {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr())
      {
        turbulence->correct();
      }
    }
    #include "log_summary.inc"
    rho = thermo.rho();
    if (runTime.write())
    {
      combustion->dQ()().write();
    }
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
