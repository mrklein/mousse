// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "psi_combustion_model.hpp"
#include "multivariate_scheme.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "fvc_smooth.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_time_controls.inc"
  #include "create_rdelta_t.inc"
  #include "init_continuity_errs.inc"
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  if (!LTS)
  {
    #include "compressible_courant_no.inc"
    #include "set_initial_delta_t.inc"
  }
    Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_time_controls.inc"
    if (LTS)
    {
      #include "set_rdelta_t.inc"
    }
    else
    {
      #include "compressible_courant_no.inc"
      #include "set_delta_t.inc"
    }
    runTime++;
    Info<< "Time = " << runTime.timeName() << nl << endl;
    #include "rho_eqn.inc"
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
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
