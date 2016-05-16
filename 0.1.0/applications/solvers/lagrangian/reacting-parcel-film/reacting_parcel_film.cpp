// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "basic_reacting_cloud.hpp"
#include "surface_film_model.hpp"
#include "psi_combustion_model.hpp"
#include "radiation_model.hpp"
#include "slg_thermo.hpp"
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
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_clouds.inc"
  #include "create_radiation_model.inc"
  #include "create_surface_film_model.inc"
  #include "init_continuity_errs.inc"
  #include "create_time_controls.inc"
  #include "compressible_courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "create_time_controls.inc"
    #include "compressible_courant_no.inc"
    #include "set_multi_region_delta_t.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    parcels.evolve();
    surfaceFilm.evolve();
    if (solvePrimaryRegion) {
      #include "rho_eqn.inc"
      // --- PIMPLE loop
      while (pimple.loop()) {
        #include "u_eqn.inc"
        #include "y_eqn.inc"
        #include "e_eqn.inc"
        // --- Pressure corrector loop
        while (pimple.correct()) {
          #include "p_eqn.inc"
        }
        if (pimple.turbCorr()) {
          turbulence->correct();
        }
      }
      rho = thermo.rho();
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End" << endl;
  return 0;
}

