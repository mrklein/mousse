// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "laminar_flame_speed.hpp"
#include "xi_model.hpp"
#include "pdr_drag_model.hpp"
#include "ignition.hpp"
#include "switch.hpp"
#include "bound.hpp"
#include "pimple_control.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "read_combustion_properties.inc"
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "init_continuity_errs.inc"
  #include "create_time_controls.inc"
  #include "compressible_courant_no.inc"
  #include "set_initial_delta_t.inc"
  scalar StCoNum = 0.0;
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "create_time_controls.inc"
    #include "compressible_courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "\n\nTime = " << runTime.timeName() << endl;
    #include "rho_eqn.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "u_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "b_eqn.inc"
        #include "ft_eqn.inc"
        #include "eau_eqn.inc"
        #include "ea_eqn.inc"
        if (!ign.ignited()) {
          thermo.heu() == thermo.he();
        }
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
        turbulence->correct();
      }
    }
    runTime.write();
    Info << "\nExecutionTime = "
      << runTime.elapsedCpuTime()
      << " s\n" << endl;
  }
  Info << "\n end\n";
  return 0;
}

