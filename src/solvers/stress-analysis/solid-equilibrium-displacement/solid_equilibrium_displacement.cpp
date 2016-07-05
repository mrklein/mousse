// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "switch.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "read_mechanical_properties.inc"
  #include "create_fields.inc"
  Info << "\nCalculating displacement field\n" << endl;
  while (runTime.loop()) {
    Info << "Iteration: " << runTime.value() << nl << endl;
    #include "read_steady_stress_controls.inc"
    solve
    (
      fvm::laplacian(2*mu + lambda, Dcorr, "laplacian(DD,Dcorr)")
    + fvc::div(sigmaExp + sigmaD)
    );
    D += accFac*Dcorr;

    {
      volTensorField gradDcorr(fvc::grad(Dcorr));
      sigmaExp =
        (lambda - mu)*gradDcorr + mu*gradDcorr.T() + (lambda*I)*tr(gradDcorr);
      sigmaD += accFac*(mu*twoSymm(gradDcorr) + (lambda*I)*tr(gradDcorr));
    }
    #include "calculate_stress.inc"
    #include "kinetic_energy_limiter.inc"
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

