// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "multiphase_system.hpp"
#include "phase_model.hpp"
#include "drag_model.hpp"
#include "heat_transfer_model.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"
#include "io_mrf_zone_list.hpp"
#include "correct_phi.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_fields.inc"
  #include "create_mrf_zones.inc"
  #include "init_continuity_errs.inc"
  #include "create_time_controls.inc"
  #include "correct_phi.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"

  Info << "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "create_time_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      turbulence->correct();
      fluid.solve();
      rho = fluid.rho();
      #include "zone_phase_volumes.inc"
      //#include "t_eqns.inc"
      #include "u_eqns.inc"
      // --- Pressure corrector loop
      while (pimple.correct())
      {
        #include "p_eqn.inc"
      }
      #include "d_dt_u.inc"
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
