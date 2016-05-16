// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "solid_thermo.hpp"
#include "wall_fv_patch.hpp"


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"

  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    mesh.readUpdate();
    #include "create_fields.inc"
    surfaceScalarField heatFlux
    {
      fvc::interpolate
      (
        turbulence.valid() ? turbulence->alphaEff()() : thermo->alpha()
      )*fvc::snGrad(h)
    };
    const surfaceScalarField::GeometricBoundaryField& patchHeatFlux =
      heatFlux.boundaryField();
    Info << "\nWall heat fluxes [W]" << endl;
    FOR_ALL(patchHeatFlux, patchi) {
      if (isA<wallFvPatch>(mesh.boundary()[patchi])) {
        Info << mesh.boundary()[patchi].name()
          << " "
          << gSum(mesh.magSf().boundaryField()[patchi]*patchHeatFlux[patchi])
          << endl;
      }
    }
    Info << endl;
    volScalarField wallHeatFlux
    {
      {
        "wallHeatFlux",
        runTime.timeName(),
        mesh
      },
      mesh,
      {"wallHeatFlux", heatFlux.dimensions(), 0.0}
    };
    FOR_ALL(wallHeatFlux.boundaryField(), patchi) {
      wallHeatFlux.boundaryField()[patchi] = patchHeatFlux[patchi];
    }
    wallHeatFlux.write();
  }
  Info << "End" << endl;
  return 0;
}

