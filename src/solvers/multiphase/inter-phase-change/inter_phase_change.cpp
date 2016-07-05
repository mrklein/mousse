// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "cmules.hpp"
#include "sub_cycle.hpp"
#include "interface_properties.hpp"
#include "phase_change_two_phase_mixture.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_fields.inc"
  #include "create_fv_options.inc"
  #include "create_time_controls.inc"
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
      #include "alpha_controls.inc"
      surfaceScalarField rhoPhi
      {
        {
          "rhoPhi",
          runTime.timeName(),
          mesh
        },
        mesh,
        {"0", dimMass/dimTime, 0}
      };
      mixture->correct();
      #include "alpha_eqn_sub_cycle.inc"
      interface.correct();
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

