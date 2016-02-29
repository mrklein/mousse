// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "engine_time.hpp"
#include "engine_mesh.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "laminar_flame_speed.hpp"
#include "ignition.hpp"
#include "switch.hpp"
#include "ofstream.hpp"
#include "mathematical_constants.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_engine_time.inc"
  #include "create_engine_mesh.inc"
  pimpleControl pimple{mesh};
  #include "read_combustion_properties.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_rho_uf.inc"
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
    #include "rho_eqn.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "u_eqn.inc"
      #include "ft_eqn.inc"
      #include "b_eqn.inc"
      #include "eau_eqn.inc"
      #include "ea_eqn.inc"
      if (!ign.ignited())
      {
        thermo.heu() == thermo.he();
      }
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
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
