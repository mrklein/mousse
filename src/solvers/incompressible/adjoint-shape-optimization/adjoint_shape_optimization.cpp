// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "simple_control.hpp"


template<class Type>
void zeroCells
(
  GeometricField<Type, fvPatchField, volMesh>& vf,
  const labelList& cells
)
{
  FOR_ALL(cells, i) {
    vf[cells[i]] = pTraits<Type>::zero;
  }
}


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  simpleControl simple{mesh};
  #include "create_fields.inc"
  #include "init_continuity_errs.inc"
  #include "init_adjoint_continuity_errs.inc"
  Info << "\nStarting time loop\n" << endl;
  while (simple.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    laminarTransport.lookup("lambda") >> lambda;
    //alpha +=
    //    mesh.relaxationFactor("alpha")
    //   *(lambda*max(Ua & U, zeroSensitivity) - alpha);
    alpha += mesh.fieldRelaxationFactor("alpha")
      *(min(max(alpha + lambda*(Ua & U), zeroAlpha), alphaMax) - alpha);
    zeroCells(alpha, inletCells);
    // Pressure-velocity SIMPLE corrector
    {
      // Momentum predictor
      tmp<fvVectorMatrix> UEqn
      {
        fvm::div(phi, U)
      + turbulence->divDevReff(U)
      + fvm::Sp(alpha, U)
      };
      UEqn().relax();
      solve(UEqn() == -fvc::grad(p));
      volScalarField rAU{1.0/UEqn().A()};
      volVectorField HbyA{"HbyA", U};
      HbyA = rAU*UEqn().H();
      UEqn.clear();
      surfaceScalarField phiHbyA
      {
        "phiHbyA",
        fvc::interpolate(HbyA) & mesh.Sf()
      };
      adjustPhi(phiHbyA, U, p);
      // Non-orthogonal pressure corrector loop
      while (simple.correctNonOrthogonal()) {
        fvScalarMatrix pEqn{fvm::laplacian(rAU, p) == fvc::div(phiHbyA)};
        pEqn.setReference(pRefCell, pRefValue);
        pEqn.solve();
        if (simple.finalNonOrthogonalIter())
          phi = phiHbyA - pEqn.flux();
      }
      #include "continuity_errs.inc"
      // Explicitly relax pressure for momentum corrector
      p.relax();
      // Momentum corrector
      U = HbyA - rAU*fvc::grad(p);
      U.correctBoundaryConditions();
    }
    // Adjoint Pressure-velocity SIMPLE corrector
    {
      // Adjoint Momentum predictor
      volVectorField adjointTransposeConvection{(fvc::grad(Ua) & U)};
      zeroCells(adjointTransposeConvection, inletCells);
      tmp<fvVectorMatrix> UaEqn
      {
        fvm::div(-phi, Ua)
      - adjointTransposeConvection
      + turbulence->divDevReff(Ua)
      + fvm::Sp(alpha, Ua)
      };
      UaEqn().relax();
      solve(UaEqn() == -fvc::grad(pa));
      volScalarField rAUa{1.0/UaEqn().A()};
      volVectorField HbyAa{"HbyAa", Ua};
      HbyAa = rAUa*UaEqn().H();
      UaEqn.clear();
      surfaceScalarField phiHbyAa
      {
        "phiHbyAa",
        fvc::interpolate(HbyAa) & mesh.Sf()
      };
      adjustPhi(phiHbyAa, Ua, pa);
      // Non-orthogonal pressure corrector loop
      while (simple.correctNonOrthogonal()) {
        fvScalarMatrix paEqn{fvm::laplacian(rAUa, pa) == fvc::div(phiHbyAa)};
        paEqn.setReference(paRefCell, paRefValue);
        paEqn.solve();
        if (simple.finalNonOrthogonalIter())
          phia = phiHbyAa - paEqn.flux();
      }
      #include "adjoint_continuity_errs.inc"
      // Explicitly relax pressure for adjoint momentum corrector
      pa.relax();
      // Adjoint momentum corrector
      Ua = HbyAa - rAUa*fvc::grad(pa);
      Ua.correctBoundaryConditions();
    }
    laminarTransport.correct();
    turbulence->correct();
    runTime.write();
    Info << "ExecutionTime = "
      << runTime.elapsedCpuTime()
      << " s\n\n" << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

