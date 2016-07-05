// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "mules.hpp"
#include "sub_cycle.hpp"
#include "incompressible_two_phase_mixture.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "init_continuity_errs.inc"
  #include "create_fields.inc"
  #include "create_time_controls.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "create_time_controls.inc"
    #include "courant_no.inc"
    #include "alpha_courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    mixture.correct();
    #include "alpha_eqn_sub_cycle.inc"
    #include "alpha_diffusion_eqn.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "u_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
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

