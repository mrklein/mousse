// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "two_phase_system.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"
#include "fixed_value_fvs_patch_fields.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_time_controls.inc"
  #include "courant_nos.inc"
  #include "set_initial_delta_t.inc"

  Switch faceMomentum
  {
    pimple.dict().lookupOrDefault<Switch>("faceMomentum", false)
  };
  Switch implicitPhasePressure
  {
    mesh.solverDict(alpha1.name()).lookupOrDefault<Switch>
    (
      "implicitPhasePressure", false
    )
  };
  #include "p_uf/create_d_dt_u.inc"
  #include "p_u/create_d_dt_u.inc"

  Info << "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "create_time_controls.inc"
    #include "courant_nos.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      fluid.solve();
      fluid.correct();
      #include "cont_errs.inc"
      if (faceMomentum)
      {
        #include "p_uf/u_eqns.inc"
        #include "e_eqns.inc"
        #include "p_uf/p_eqn.inc"
        #include "p_uf/d_dt_u.inc"
      }
      else
      {
        #include "p_u/u_eqns.inc"
        #include "e_eqns.inc"
        #include "p_u/p_eqn.inc"
        #include "p_u/d_dt_u.inc"
      }
      if (pimple.turbCorr())
      {
        fluid.correctTurbulence();
      }
    }
    #include "write.inc"
    Info << "ExecutionTime = "
      << runTime.elapsedCpuTime()
      << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
