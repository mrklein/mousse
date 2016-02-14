// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dynamic_fv_mesh.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "laminar_flame_speed.hpp"
#include "xi_model.hpp"
#include "pdr_drag_model.hpp"
#include "ignition.hpp"
#include "switch.hpp"
#include "bound.hpp"
#include "dynamic_refine_fv_mesh.hpp"
#include "pimple_control.hpp"
int main(int argc, char *argv[])
{
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  #include "create_dynamic_fv_mesh.hpp"
  pimpleControl pimple(mesh);
  #include "read_combustion_properties.hpp"
  #include "read_gravitational_acceleration.hpp"
  #include "create_fields.hpp"
  #include "init_continuity_errs.hpp"
  #include "create_time_controls.hpp"
  #include "compressible_courant_no.hpp"
  #include "set_initial_delta_t.hpp"
  scalar StCoNum = 0.0;
    Info<< "\nStarting time loop\n" << endl;
  bool hasChanged = false;
  while (runTime.run())
  {
    #include "create_time_controls.hpp"
    #include "compressible_courant_no.hpp"
    #include "set_delta_t.hpp"
    // Indicators for refinement. Note: before runTime++
    // only for postprocessing reasons.
    tmp<volScalarField> tmagGradP = mag(fvc::grad(p));
    volScalarField normalisedGradP
    (
      "normalisedGradP",
      tmagGradP()/max(tmagGradP())
    );
    normalisedGradP.writeOpt() = IOobject::AUTO_WRITE;
    tmagGradP.clear();
    runTime++;
    Info<< "\n\nTime = " << runTime.timeName() << endl;
    {
      // Make the fluxes absolute
      fvc::makeAbsolute(phi, rho, U);
      // Test : disable refinement for some cells
      PackedBoolList& protectedCell =
        refCast<dynamicRefineFvMesh>(mesh).protectedCell();
      if (protectedCell.empty())
      {
        protectedCell.setSize(mesh.nCells());
        protectedCell = 0;
      }
      forAll(betav, cellI)
      {
        if (betav[cellI] < 0.99)
        {
          protectedCell[cellI] = 1;
        }
      }
      // Flux estimate for introduced faces.
      volVectorField rhoU("rhoU", rho*U);
      // Do any mesh changes
      bool meshChanged = mesh.update();
      if (meshChanged)
      {
        hasChanged = true;
      }
      if (runTime.write() && hasChanged)
      {
        betav.write();
        Lobs.write();
        CT.write();
        drag->writeFields();
        flameWrinkling->writeFields();
        hasChanged = false;
      }
      // Make the fluxes relative to the mesh motion
      fvc::makeRelative(phi, rho, U);
    }
    #include "rho_eqn.hpp"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop())
    {
      #include "u_eqn.hpp"
      // --- Pressure corrector loop
      while (pimple.correct())
      {
        #include "b_eqn.hpp"
        #include "ft_eqn.hpp"
        #include "hu_eqn.hpp"
        #include "h_eqn.hpp"
        if (!ign.ignited())
        {
          hu == h;
        }
        #include "p_eqn.hpp"
      }
      if (pimple.turbCorr())
      {
        turbulence->correct();
      }
    }
    runTime.write();
    Info<< "\nExecutionTime = "
      << runTime.elapsedCpuTime()
      << " s\n" << endl;
  }
  Info<< "\n end\n";
  return 0;
}
