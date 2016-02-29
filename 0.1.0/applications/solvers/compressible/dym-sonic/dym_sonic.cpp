// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "psi_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "pimple_control.hpp"
#include "correct_phi.hpp"
#include "fv_io_option_list.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_dynamic_fv_mesh.inc"
  #include "init_continuity_errs.inc"
  pimpleControl pimple{mesh};
  #include "create_controls.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_rho_uf.inc"
  #include "compressible_courant_no.inc"
  #include "set_initial_delta_t.inc"

  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_controls.inc"

    {
      // Store divrhoU from the previous mesh so that it can be mapped
      // and used in correctPhi to ensure the corrected phi has the
      // same divergence
      volScalarField divrhoU
      {
        "divrhoU",
        fvc::div(fvc::absolute(phi, rho, U))
      };
      #include "compressible_courant_no.inc"
      #include "set_delta_t.inc"
      runTime++;
      Info<< "Time = " << runTime.timeName() << nl << endl;
      // Store momentum to set rhoUf for introduced faces.
      volVectorField rhoU{"rhoU", rho*U};
      // Do any mesh changes
      mesh.update();
      if (mesh.changing() && correctPhi)
      {
        // Calculate absolute flux from the mapped surface velocity
        phi = mesh.Sf() & rhoUf;
        #include "correct_phi.inc"
        // Make the fluxes relative to the mesh-motion
        fvc::makeRelative(phi, rho, U);
      }
    }
    if (mesh.changing() && checkMeshCourantNo)
    {
      #include "mesh_courant_no.inc"
    }
    #include "rho_eqn.inc"
    Info<< "rhoEqn max/min : " << max(rho).value()
      << " " << min(rho).value() << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "u_eqn.inc"
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
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
