// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "rho_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "radiation_model.hpp"
#include "simple_control.hpp"
#include "fv_io_option_list.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  simpleControl simple(mesh);
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_radiation_model.inc"
  #include "init_continuity_errs.inc"

  Info<< "\nStarting time loop\n" << endl;
  while (simple.loop())
  {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    // Pressure-velocity SIMPLE corrector
    {
      #include "u_eqn.inc"
      #include "e_eqn.inc"
      #include "p_eqn.inc"
    }
    turbulence->correct();
    runTime.write();
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
