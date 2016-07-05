// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "mules.hpp"
#include "sub_cycle.hpp"
#include "interface_properties.hpp"
#include "two_phase_mixture.hpp"
#include "two_phase_mixture_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "pimple_control.hpp"
#include "correct_phi.hpp"
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
  #include "create_controls.inc"
  #include "courant_no.inc"
  #include "set_initial_delta_t.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "read_controls.inc"
    {
      // Store divU from the previous mesh so that it can be mapped
      // and used in correctPhi to ensure the corrected phi has the
      // same divergence
      volScalarField divU{"divU0", fvc::div(fvc::absolute(phi, U))};
      #include "courant_no.inc"
      #include "set_delta_t.inc"
      runTime++;
      Info << "Time = " << runTime.timeName() << nl << endl;
      scalar timeBeforeMeshUpdate = runTime.elapsedCpuTime();
      // Do any mesh changes
      mesh.update();
      if (mesh.changing()) {
        Info << "Execution time for mesh.update() = "
          << runTime.elapsedCpuTime() - timeBeforeMeshUpdate
          << " s" << endl;
        gh = (g & mesh.C()) - ghRef;
        ghf = (g & mesh.Cf()) - ghRef;
      }
      if (mesh.changing() && correctPhi) {
        // Calculate absolute flux from the mapped surface velocity
        phi = mesh.Sf() & Uf;
        #include "correct_phi.inc"
        // Make the fluxes relative to the mesh motion
        fvc::makeRelative(phi, U);
      }
    }
    if (mesh.changing() && checkMeshCourantNo) {
      #include "mesh_courant_no.inc"
    }
    turbulence->correct();
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "alpha_eqns_sub_cycle.inc"
      // correct interface on first PIMPLE corrector
      if (pimple.corr() == 1) {
        interface.correct();
      }
      solve(fvm::ddt(rho) + fvc::div(rhoPhi));
      #include "u_eqn.inc"
      #include "t_eqn.inc"
      // --- Pressure corrector loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
    }
    rho = alpha1*rho1 + alpha2*rho2;
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

