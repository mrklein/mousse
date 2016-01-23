// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "piso_control.hpp"
#include "fv_io_option_list.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pisoControl piso(mesh);
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "init_continuity_errs.inc"
  Info<< "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    #include "courant_no.inc"
    // Pressure-velocity PISO corrector
    {
      #include "u_eqn.inc"
      // --- PISO loop
      while (piso.correct())
      {
        #include "p_eqn.inc"
      }
    }
    laminarTransport.correct();
    turbulence->correct();
    runTime.write();
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
