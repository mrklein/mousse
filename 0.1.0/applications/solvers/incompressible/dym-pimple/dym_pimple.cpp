// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"
#include "correct_phi.hpp"
#include "fv_io_option_list.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_dynamic_fv_mesh.inc"
  #include "init_continuity_errs.inc"
  pimpleControl pimple{mesh};
  #include "create_fields.inc"
  #include "create_uf.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_controls.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "read_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    mesh.update();
    // Calculate absolute flux from the mapped surface velocity
    phi = mesh.Sf() & Uf;
    if (mesh.changing() && correctPhi) {
      #include "correct_phi.inc"
    }
    // Make the flux relative to the mesh motion
    fvc::makeRelative(phi, U);
    if (mesh.changing() && checkMeshCourantNo) {
      #include "mesh_courant_no.inc"
    }
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "u_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
        laminarTransport.correct();
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
