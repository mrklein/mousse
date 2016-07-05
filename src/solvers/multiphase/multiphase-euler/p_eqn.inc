{
  PtrList<surfaceScalarField> alphafs{fluid.phases().size()};
  PtrList<volVectorField> HbyAs{fluid.phases().size()};
  PtrList<surfaceScalarField> phiHbyAs{fluid.phases().size()};
  PtrList<volScalarField> rAUs{fluid.phases().size()};
  PtrList<surfaceScalarField> rAlphaAUfs{fluid.phases().size()};
  phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
  {
    phaseModel& phase = iter();
    MRF.makeAbsolute(phase.phi().oldTime());
    MRF.makeAbsolute(phase.phi());
    HbyAs.set(phasei, new volVectorField(phase.U()));
    phiHbyAs.set(phasei, new surfaceScalarField(1.0*phase.phi()));
    phasei++;
  }
  surfaceScalarField phiHbyA
  {
    {
      "phiHbyA",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh,
    {"phiHbyA", dimArea*dimVelocity, 0}
  };
  volScalarField rho{"rho", fluid.rho()};
  surfaceScalarField ghSnGradRho{ghf*fvc::snGrad(rho)*mesh.magSf()};
  phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
  {
    phaseModel& phase = iter();
    const volScalarField& alpha = phase;
    alphafs.set(phasei, fvc::interpolate(alpha).ptr());
    alphafs[phasei].rename("hmm" + alpha.name());
    volScalarField dragCoeffi
    {
      {
        "dragCoeffi",
        runTime.timeName(),
        mesh
      },
      fluid.dragCoeff(phase, dragCoeffs())/phase.rho(),
      zeroGradientFvPatchScalarField::typeName
    };
    dragCoeffi.correctBoundaryConditions();
    rAUs.set(phasei, (1.0/(UEqns[phasei].A() + dragCoeffi)).ptr());
    rAlphaAUfs.set
    (
      phasei,
      (
        alphafs[phasei]/fvc::interpolate(UEqns[phasei].A() + dragCoeffi)
      ).ptr()
    );
    HbyAs[phasei] = rAUs[phasei]*UEqns[phasei].H();
    phiHbyAs[phasei] =
    (
      (fvc::interpolate(HbyAs[phasei]) & mesh.Sf())
      + rAlphaAUfs[phasei]*fvc::ddtCorr(phase.U(), phase.phi())
    );
    MRF.makeRelative(phiHbyAs[phasei]);
    MRF.makeRelative(phase.phi().oldTime());
    MRF.makeRelative(phase.phi());
    phiHbyAs[phasei] +=
      rAlphaAUfs[phasei]
      *(
        fluid.surfaceTension(phase)*mesh.magSf()
        + (phase.rho() - fvc::interpolate(rho))*(g & mesh.Sf())
        - ghSnGradRho
      )/phase.rho();
    multiphaseSystem::dragModelTable::const_iterator dmIter =
      fluid.dragModels().begin();
    multiphaseSystem::dragCoeffFields::const_iterator dcIter =
      dragCoeffs().begin();
    for
    (
      ;
      dmIter != fluid.dragModels().end() && dcIter != dragCoeffs().end();
      ++dmIter, ++dcIter
    )
    {
      const phaseModel *phase2Ptr = NULL;
      if (&phase == &dmIter()->phase1())
      {
        phase2Ptr = &dmIter()->phase2();
      }
      else if (&phase == &dmIter()->phase2())
      {
        phase2Ptr = &dmIter()->phase1();
      }
      else
      {
        continue;
      }
      phiHbyAs[phasei] +=
        fvc::interpolate((*dcIter())/phase.rho())
        /fvc::interpolate(UEqns[phasei].A() + dragCoeffi)
        *phase2Ptr->phi();
      HbyAs[phasei] +=
        (1.0/phase.rho())*rAUs[phasei]*(*dcIter())
        *phase2Ptr->U();
    }
    phiHbyA += alphafs[phasei]*phiHbyAs[phasei];
    phasei++;
  }
  surfaceScalarField rAUf
  {
    {
      "rAUf",
      runTime.timeName(),
      mesh
    },
    mesh,
    {"rAUf", dimensionSet(-1, 3, 1, 0, 0), 0}
  };
  phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
  {
    phaseModel& phase = iter();
    rAUf += mag(alphafs[phasei]*rAlphaAUfs[phasei])/phase.rho();
    phasei++;
  }

  // Update the fixedFluxPressure BCs to ensure flux consistency
  {
    surfaceScalarField::GeometricBoundaryField phib(phi.boundaryField());
    phib = 0;
    phasei = 0;
    FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
    {
      phaseModel& phase = iter();
      phib += alphafs[phasei].boundaryField()
        *(mesh.Sf().boundaryField() & phase.U().boundaryField());
      phasei++;
    }
    setSnGrad<fixedFluxPressureFvPatchScalarField>
    (
      p_rgh.boundaryField(),
      (
        phiHbyA.boundaryField() - MRF.relative(phib)
      )/(mesh.magSf().boundaryField()*rAUf.boundaryField())
    );
  }
  while (pimple.correctNonOrthogonal())
  {
    fvScalarMatrix pEqnIncomp
    {
      fvc::div(phiHbyA) - fvm::laplacian(rAUf, p_rgh)
    };
    pEqnIncomp.setReference(pRefCell, pRefValue);
    solve
    (
      pEqnIncomp,
      mesh.solver(p_rgh.select(pimple.finalInnerIter()))
    );
    if (pimple.finalNonOrthogonalIter())
    {
      surfaceScalarField mSfGradp{"mSfGradp", pEqnIncomp.flux()/rAUf};
      phasei = 0;
      phi = dimensionedScalar{"phi", phi.dimensions(), 0};
      FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
      {
        phaseModel& phase = iter();
        phase.phi() = phiHbyAs[phasei] + rAlphaAUfs[phasei]*mSfGradp/phase.rho();
        phi += (alphafs[phasei]*phiHbyAs[phasei]
          + mag(alphafs[phasei]*rAlphaAUfs[phasei])*mSfGradp/phase.rho());
        phasei++;
      }
      // dgdt =
      // (
      //     pos(alpha2)*(pEqnComp2 & p)/rho2
      //   - pos(alpha1)*(pEqnComp1 & p)/rho1
      // );
      p_rgh.relax();
      p = p_rgh + rho*gh;
      mSfGradp = pEqnIncomp.flux()/rAUf;
      U = dimensionedVector{"U", dimVelocity, vector::zero};
      phasei = 0;
      FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
      {
        phaseModel& phase = iter();
        const volScalarField& alpha = phase;
        phase.U() =
          HbyAs[phasei]
          + fvc::reconstruct
          (
            rAlphaAUfs[phasei]
            *(
              (phase.rho() - fvc::interpolate(rho))
              *(g & mesh.Sf())
              - ghSnGradRho
              + mSfGradp
            )
          )/phase.rho();
        //phase.U() = fvc::reconstruct(phase.phi());
        phase.U().correctBoundaryConditions();
        U += alpha*phase.U();
        phasei++;
      }
    }
  }
  #include "continuity_errs.inc"
}
