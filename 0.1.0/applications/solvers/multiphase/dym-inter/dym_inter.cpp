// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "cmules.hpp"
#include "euler_ddt_scheme.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "crank_nicolson_ddt_scheme.hpp"
#include "sub_cycle.hpp"
#include "immiscible_incompressible_two_phase_mixture.hpp"
#include "turbulent_transport_model.hpp"
#include "pimple_control.hpp"
#include "fv_io_option_list.hpp"
#include "correct_phi.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "fvc_smooth.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_dynamic_fv_mesh.inc"
  #include "init_continuity_errs.inc"
  pimpleControl pimple{mesh};
  #include "create_controls.inc"
  #include "create_rdelta_t.inc"
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"

  volScalarField rAU
  {
    {
      "rAU",
      runTime.timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh,
    {"rAUf", dimTime/rho.dimensions(), 1.0}
  };
  #include "correct_phi.inc"
  #include "create_uf.inc"
  if (!LTS)
  {
    #include "courant_no.inc"
    #include "set_initial_delta_t.inc"
  }
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    #include "read_controls.inc"
    if (LTS)
    {
      #include "set_rdelta_t.inc"
    }
    else
    {
      #include "courant_no.inc"
      #include "alpha_courant_no.inc"
      #include "set_delta_t.inc"
    }
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      if (pimple.firstIter() || moveMeshOuterCorrectors)
      {
        scalar timeBeforeMeshUpdate = runTime.elapsedCpuTime();
        mesh.update();
        if (mesh.changing())
        {
          Info << "Execution time for mesh.update() = "
            << runTime.elapsedCpuTime() - timeBeforeMeshUpdate
            << " s" << endl;
          gh = (g & mesh.C()) - ghRef;
          ghf = (g & mesh.Cf()) - ghRef;
        }
        if (mesh.changing() && correctPhi)
        {
          // Calculate absolute flux from the mapped surface velocity
          phi = mesh.Sf() & Uf;
          #include "correct_phi.inc"
          // Make the flux relative to the mesh motion
          fvc::makeRelative(phi, U);
          mixture.correct();
        }
        if (mesh.changing() && checkMeshCourantNo)
        {
          #include "mesh_courant_no.inc"
        }
      }
      #include "alpha_controls.inc"
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
