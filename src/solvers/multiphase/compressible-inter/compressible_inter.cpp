// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "mules.hpp"
#include "sub_cycle.hpp"
#include "rho_thermo.hpp"
#include "interface_properties.hpp"
#include "two_phase_mixture.hpp"
#include "two_phase_mixture_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "pimple_control.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_time_controls.inc"
  #include "create_fields.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "create_time_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "alpha_eqns_sub_cycle.inc"
      // correct interface on first PIMPLE corrector
      if (pimple.corr() == 1) {
        interface.correct();
      }
      solve(fvm::ddt(rho) + fvc::div(rhoPhi));
      #include "u_eqn.inc"
      #include "t_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
        turbulence->correct();
      }
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
