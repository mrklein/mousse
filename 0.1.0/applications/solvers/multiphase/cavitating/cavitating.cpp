// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "barotropic_compressibility_model.hpp"
#include "incompressible_two_phase_mixture.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "read_thermodynamic_properties.inc"
  #include "create_controls.inc"
  #include "create_fields.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"

  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info<< "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "rho_eqn.inc"
      #include "alphav_psi.inc"
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
