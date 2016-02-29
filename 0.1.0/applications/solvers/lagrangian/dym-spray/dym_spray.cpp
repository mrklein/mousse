// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "turbulence_model.hpp"
#include "basic_spray_cloud.hpp"
#include "psi_combustion_model.hpp"
#include "radiation_model.hpp"
#include "slg_thermo.hpp"
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
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "create_rho_uf.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  #include "create_clouds.inc"
  #include "create_radiation_model.inc"
  #include "compressible_courant_no.inc"
  #include "set_initial_delta_t.inc"

  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_controls.inc"

    {
      // Store divrhoU from the previous time-step/mesh for the correctPhi
      volScalarField divrhoU
      {
        "divrhoU",
        fvc::div(fvc::absolute(phi, rho, U))
      };
      #include "compressible_courant_no.inc"
      #include "set_delta_t.inc"
      runTime++;
      Info << "Time = " << runTime.timeName() << nl << endl;
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
    parcels.evolve();
    #include "rho_eqn.inc"
    // --- Pressure-velocity PIMPLE corrector loop
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
    rho = thermo.rho();
    if (runTime.write())
    {
      combustion->dQ()().write();
    }
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
