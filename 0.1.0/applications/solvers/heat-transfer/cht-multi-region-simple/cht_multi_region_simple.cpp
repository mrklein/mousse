// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "rho_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "fixed_gradient_fv_patch_fields.hpp"
#include "region_properties.hpp"
#include "solid_thermo.hpp"
#include "radiation_model.hpp"
#include "fv_io_option_list.hpp"
#include "coordinate_system.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  regionProperties rp{runTime};
  #include "create_fluid_meshes.inc"
  #include "create_solid_meshes.inc"
  #include "create_fluid_fields.inc"
  #include "create_solid_fields.inc"
  #include "init_continuity_errs.inc"
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    FOR_ALL(fluidRegions, i) {
      Info << "\nSolving for fluid region " << fluidRegions[i].name() << endl;
      #include "set_region_fluid_fields.inc"
      #include "read_fluid_multi_region_simple_controls.inc"
      #include "solve_fluid.inc"
    }
    FOR_ALL(solidRegions, i) {
      Info << "\nSolving for solid region " << solidRegions[i].name() << endl;
      #include "set_region_solid_fields.inc"
      #include "read_solid_multi_region_simple_controls.inc"
      #include "solve_solid.inc"
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

