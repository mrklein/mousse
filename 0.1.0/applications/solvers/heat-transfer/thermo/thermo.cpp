// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "rho_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "les_model.hpp"
#include "radiation_model.hpp"
#include "fv_io_option_list.hpp"
#include "simple_control.hpp"
#include "pimple_control.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"
  #include "create_fv_options.inc"
  #include "create_radiation_model.inc"

  Info<< "\nEvolving thermodynamics\n" << endl;
  if (mesh.solutionDict().found("SIMPLE"))
  {
    simpleControl simple(mesh);
    while (simple.loop())
    {
      Info<< "Time = " << runTime.timeName() << nl << endl;
      while (simple.correctNonOrthogonal())
      {
        #include "e_eqn.inc"
      }
      Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;
      runTime.write();
    }
  }
  else
  {
    pimpleControl pimple(mesh);
    while (runTime.run())
    {
      runTime++;
      Info<< "Time = " << runTime.timeName() << nl << endl;
      while (pimple.correctNonOrthogonal())
      {
        #include "e_eqn.inc"
      }
      Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;
      runTime.write();
    }
  }
  Info<< "End\n" << endl;
  return 0;
}
