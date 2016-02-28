#include "mrf_correct_bcs.inc"
PtrList<fvVectorMatrix> UEqns{fluid.phases().size()};
autoPtr<multiphaseSystem::dragCoeffFields> dragCoeffs{fluid.dragCoeffs()};
int phasei = 0;
FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
{
  phaseModel& phase = iter();
  const volScalarField& alpha = phase;
  volVectorField& U = phase.U();
  volScalarField nuEff{turbulence->nut() + iter().nu()};
  UEqns.set
  (
    phasei,
    new fvVectorMatrix
    {
      fvm::ddt(alpha, U)
      + fvm::div(phase.alphaPhi(), U)
      + (alpha/phase.rho())*fluid.Cvm(phase)*
      (
        fvm::ddt(U)
        + fvm::div(phase.phi(), U)
        - fvm::Sp(fvc::div(phase.phi()), U)
      )
      - fvm::laplacian(alpha*nuEff, U)
      - fvc::div
      (
        alpha*(nuEff*dev(T(fvc::grad(U)))),
        "div(Rc)"
      )
      ==
      (alpha/phase.rho())*fluid.Svm(phase)
      - fvm::Sp
      (
        slamDampCoeff
        *max
        (
          mag(U.dimensionedInternalField()) - maxSlamVelocity,
          dimensionedScalar("U0", dimVelocity, 0)
        )
        /pow(mesh.V(), 1.0/3.0),
        U
      )
    }
  );
  MRF.addAcceleration
  (
    alpha*(1 + (1/phase.rho())*fluid.Cvm(phase)),
    UEqns[phasei]
  );
  //UEqns[phasei].relax();
  phasei++;
}
