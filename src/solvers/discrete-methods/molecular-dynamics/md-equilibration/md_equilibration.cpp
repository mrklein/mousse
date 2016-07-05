// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "md.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  Info << "\nReading field U\n" << endl;
  volVectorField U
  {
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  };
  potential pot{mesh};
  moleculeCloud molecules{mesh, pot};
  #include "temperature_and_pressure_variables.inc"
  #include "readmd_equilibration_dict.inc"
  label nAveragingSteps = 0;
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    nAveragingSteps++;
    Info << "Time = " << runTime.timeName() << endl;
    molecules.evolve();
    #include "mean_momentum_energy_and_n_mols.inc"
    #include "temperature_and_pressure.inc"
    #include "temperature_equilibration.inc"
    runTime.write();
    if (runTime.outputTime()) {
      nAveragingSteps = 0;
    }
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

