// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"
  Info << "\nStarting iteration loop\n" << endl;
  while (runTime.loop()) {
    Info << "Iteration = " << runTime.timeName() << nl << endl;
    solve
    (
      fvm::laplacian(phi) + rho/epsilon0
    );
    rhoFlux = -k*mesh.magSf()*fvc::snGrad(phi);
    solve
    (
      fvm::ddt(rho) + fvm::div(rhoFlux, rho)
    );
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

