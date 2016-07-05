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
  #include "read_thermal_properties.inc"
  #include "create_solid_displacement_controls.inc"
  #include "create_fields.inc"
  Info << "\nCalculating displacement field\n" << endl;
  while (runTime.loop()) {
    Info << "Iteration: " << runTime.value() << nl << endl;
    #include "read_solid_displacement_controls.inc"
    int iCorr = 0;
    scalar initialResidual = 0;
    do {
      if (thermalStress) {
        volScalarField& T = Tptr();
        solve
        (
          fvm::ddt(T) == fvm::laplacian(DT, T)
        );
      }
      {
        fvVectorMatrix DEqn
        {
          fvm::d2dt2(D)
        ==
          fvm::laplacian(2*mu + lambda, D, "laplacian(DD,D)")
        + divSigmaExp
        };
        if (thermalStress) {
          const volScalarField& T = Tptr();
          DEqn += fvc::grad(threeKalpha*T);
        }
        initialResidual = DEqn.solve().initialResidual();
        if (!compactNormalStress) {
          divSigmaExp = fvc::div(DEqn.flux());
        }
      }
      {
        volTensorField gradD{fvc::grad(D)};
        sigmaD = mu*twoSymm(gradD) + (lambda*I)*tr(gradD);
        if (compactNormalStress) {
          divSigmaExp =
            fvc::div(sigmaD - (2*mu + lambda)*gradD, "div(sigmaD)");
        } else {
          divSigmaExp += fvc::div(sigmaD);
        }
      }
    } while (initialResidual > convergenceTolerance && ++iCorr < nCorr);
    #include "calculate_stress.inc"
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
