// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "phase_incompressible_turbulence_model.hpp"
#include "pimple_control.hpp"
#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"


#ifdef MPPIC
  #include "basic_kinematic_mppic_cloud.hpp"
  #define basicKinematicTypeCloud basicKinematicMPPICCloud
#else
  #include "basic_kinematic_colliding_cloud.hpp"
  #define basicKinematicTypeCloud basicKinematicCollidingCloud
#endif


int main(int argc, char *argv[])
{
  argList::addOption
  (
    "cloudName",
    "name",
    "specify alternative cloud name. default is 'kinematicCloud'"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "create_time_controls.inc"
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  #include "init_continuity_errs.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "read_time_controls.inc"
    #include "courant_no.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    continuousPhaseTransport.correct();
    muc = rhoc*continuousPhaseTransport.nu();
    Info << "Evolving " << kinematicCloud.name() << endl;
    kinematicCloud.evolve();
    // Update continuous phase volume fraction field
    alphac = max(1.0 - kinematicCloud.theta(), alphacMin);
    alphac.correctBoundaryConditions();
    alphacf = fvc::interpolate(alphac);
    alphaPhic = alphacf*phic;
    fvVectorMatrix cloudSU{kinematicCloud.SU(Uc)};
    volVectorField cloudVolSUSu
    {
      {
        "cloudVolSUSu",
        runTime.timeName(),
        mesh
      },
      mesh,
      {"0", cloudSU.dimensions()/dimVolume, vector::zero},
      zeroGradientFvPatchVectorField::typeName
    };
    cloudVolSUSu.internalField() = -cloudSU.source()/mesh.V();
    cloudVolSUSu.correctBoundaryConditions();
    cloudSU.source() = vector::zero;
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      #include "uc_eqn.inc"
      // --- PISO loop
      while (pimple.correct()) {
        #include "p_eqn.inc"
      }
      if (pimple.turbCorr()) {
        continuousPhaseTurbulence->correct();
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

