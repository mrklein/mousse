// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "multiphase_system.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"
#include "pimple_control.hpp"
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
  #include "create_fields.inc"
  if (!LTS)
  {
    #include "courant_no.inc"
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
      #include "courant_no.inc"
      #include "set_delta_t.inc"
    }
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      fluid.solve();
      fluid.correct();
      #include "y_eqns.inc"
      {
        #include "p_u/u_eqns.inc"
        #include "e_eqns.inc"
        #include "p_u/p_eqn.inc"
      }
      fluid.correctKinematics();
      if (pimple.turbCorr())
      {
        fluid.correctTurbulence();
      }
    }
    runTime.write();
    Info << "ExecutionTime = "
      << runTime.elapsedCpuTime()
      << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
