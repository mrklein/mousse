// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "piso_control.hpp"
#include "fv_io_option_list.hpp"


int main(int argc, char *argv[])
{
  argList::addOption("pName", "pName", "Name of the pressure field");
  argList::addBoolOption("initialiseUBCs", "Initialise U boundary conditions");
  argList::addBoolOption("writePhiU", "Write the velocity potential field");
  argList::addBoolOption("writep", "Calculate and write the pressure field");
  argList::addBoolOption("withFunctionObjects", "execute functionObjects");
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pisoControl potentialFlow{mesh, "potentialFlow"};
  #include "create_fields.inc"
  #include "create_mrf.inc"
  #include "create_fv_options.inc"
  Info<< nl << "Calculating potential flow" << endl;
  // Since solver contains no time loop it would never execute
  // function objects so do it ourselves
  runTime.functionObjects().start();
  MRF.makeRelative(phi);
  adjustPhi(phi, U, p);
  // Non-orthogonal velocity potential corrector loop
  while (potentialFlow.correctNonOrthogonal()) {
    fvScalarMatrix phiUEqn {
      fvm::laplacian(dimensionedScalar{"1", dimless, 1}, phiU)
    ==
      fvc::div(phi)
    };
    phiUEqn.setReference(phiURefCell, phiURefValue);
    phiUEqn.solve();
    if (potentialFlow.finalNonOrthogonalIter()) {
      phi -= phiUEqn.flux();
    }
  }
  MRF.makeAbsolute(phi);
  Info << "Continuity error = "
    << mag(fvc::div(phi))().weightedAverage(mesh.V()).value()
    << endl;
  U = fvc::reconstruct(phi);
  U.correctBoundaryConditions();
  Info << "Interpolated velocity error = "
    << (sqrt(sum(sqr((fvc::interpolate(U) & mesh.Sf()) - phi)))
        /sum(mesh.magSf())).value()
    << endl;
  // Write U and phi
  U.write();
  phi.write();
  // Optionally write phiU
  if (args.optionFound("writePhiU")) {
    phiU.write();
  }
  // Calculate the pressure field
  if (args.optionFound("writep")) {
    Info<< nl << "Calculating approximate pressure field" << endl;
    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell
    (
      p,
      potentialFlow.dict(),
      pRefCell,
      pRefValue
    );
    // Calculate the flow-direction filter tensor
    volScalarField magSqrU{magSqr(U)};
    volSymmTensorField F{sqr(U)/(magSqrU + SMALL*average(magSqrU))};
    // Calculate the divergence of the flow-direction filtered div(U*U)
    // Filtering with the flow-direction generates a more reasonable
    // pressure distribution in regions of high velocity gradient in the
    // direction of the flow
    volScalarField divDivUU
    {
      fvc::div(F & fvc::div(phi, U), "div(div(phi,U))")
    };
    // Solve a Poisson equation for the approximate pressure
    while (potentialFlow.correctNonOrthogonal()) {
      fvScalarMatrix pEqn{fvm::laplacian(p) + divDivUU};
      pEqn.setReference(pRefCell, pRefValue);
      pEqn.solve();
    }
    p.write();
  }
  runTime.functionObjects().end();
  Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  Info << "End\n" << endl;
  return 0;
}

