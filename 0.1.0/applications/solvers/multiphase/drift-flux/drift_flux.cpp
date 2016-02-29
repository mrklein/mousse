// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "cmules.hpp"
#include "sub_cycle.hpp"
#include "incompressible_two_phase_interacting_mixture.hpp"
#include "relative_velocity_model.hpp"
#include "turbulence_model.hpp"
#include "_compressible_turbulence_model.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"
#include "gauss_laplacian_scheme.hpp"
#include "uncorrected_sn_grad.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_time_controls.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "init_continuity_errs.inc"

  Info << "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_time_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "alpha_controls.inc"
      UdmModel.correct();
      #include "alpha_eqn_sub_cycle.inc"
      mixture.correct();
      #include "u_eqn.inc"
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
