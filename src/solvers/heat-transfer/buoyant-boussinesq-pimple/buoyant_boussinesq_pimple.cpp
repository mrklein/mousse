// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "radiation_model.hpp"
#include "fv_io_option_list.hpp"
#include "pimple_control.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_fields.inc"
  #include "create_incompressible_radiation_model.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "init_continuity_errs.inc"
  #include "create_time_controls.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    #include "create_time_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "u_eqn.inc"
      #include "t_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
        laminarTransport.correct();
        turbulence->correct();
      }
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

