// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "two_phase_system.hpp"
#include "drag_model.hpp"
#include "virtual_mass_model.hpp"
#include "mules.hpp"
#include "sub_cycle.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
#include "fvc_sn_grad.hpp"
#include "fvc_flux.hpp"
#include "fvc_sup.hpp"
#include "fvm_ddt.hpp"
#include "fvm_laplacian.hpp"
#include "fvm_sup.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(twoPhaseSystem, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(twoPhaseSystem, dictionary);
}
// Constructors 
mousse::twoPhaseSystem::twoPhaseSystem
(
  const fvMesh& mesh
)
:
  phaseSystem{mesh},
  phase1_{phaseModels_[0]},
  phase2_{phaseModels_[1]}
{
  phase2_.volScalarField::operator=(scalar(1) - phase1_);
  volScalarField& alpha1 = phase1_;
  mesh.setFluxRequired(alpha1.name());
}
// Destructor 
mousse::twoPhaseSystem::~twoPhaseSystem()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::twoPhaseSystem::sigma() const
{
  return sigma
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
const mousse::dragModel& mousse::twoPhaseSystem::drag(const phaseModel& phase) const
{
  return lookupSubModel<dragModel>(phase, otherPhase(phase));
}
mousse::tmp<mousse::volScalarField>
mousse::twoPhaseSystem::Kd() const
{
  return Kd
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::twoPhaseSystem::Kdf() const
{
  return Kdf
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
const mousse::virtualMassModel&
mousse::twoPhaseSystem::virtualMass(const phaseModel& phase) const
{
  return lookupSubModel<virtualMassModel>(phase, otherPhase(phase));
}
mousse::tmp<mousse::volScalarField>
mousse::twoPhaseSystem::Vm() const
{
  return Vm
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::twoPhaseSystem::Vmf() const
{
  return Vmf
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
mousse::tmp<mousse::volVectorField>
mousse::twoPhaseSystem::F() const
{
  return F
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::twoPhaseSystem::Ff() const
{
  return Ff
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
mousse::tmp<mousse::volScalarField>
mousse::twoPhaseSystem::D() const
{
  return D
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
bool mousse::twoPhaseSystem::transfersMass() const
{
  return transfersMass(phase1());
}
mousse::tmp<mousse::volScalarField>
mousse::twoPhaseSystem::dmdt() const
{
  return dmdt
  (
    phasePairKey(phase1().name(), phase2().name())
  );
}
void mousse::twoPhaseSystem::solve()
{
  const Time& runTime = mesh_.time();
  volScalarField& alpha1 = phase1_;
  volScalarField& alpha2 = phase2_;
  const dictionary& alphaControls = mesh_.solverDict(alpha1.name());
  label nAlphaSubCycles{readLabel(alphaControls.lookup("nAlphaSubCycles"))};
  label nAlphaCorr{readLabel(alphaControls.lookup("nAlphaCorr"))};
  bool LTS = fv::localEulerDdt::enabled(mesh_);
  word alphaScheme{"div(phi," + alpha1.name() + ')'};
  word alpharScheme{"div(phir," + alpha1.name() + ')'};
  const surfaceScalarField& phi = this->phi();
  const surfaceScalarField& phi1 = phase1_.phi();
  const surfaceScalarField& phi2 = phase2_.phi();
  // Construct the dilatation rate source term
  tmp<volScalarField::DimensionedInternalField> tdgdt;
  if (phase1_.divU().valid() && phase2_.divU().valid())
  {
    tdgdt =
    (
      alpha2.dimensionedInternalField()
      *phase1_.divU()().dimensionedInternalField()
      - alpha1.dimensionedInternalField()
      *phase2_.divU()().dimensionedInternalField()
    );
  }
  else if (phase1_.divU().valid())
  {
    tdgdt =
    (
      alpha2.dimensionedInternalField()
      *phase1_.divU()().dimensionedInternalField()
    );
  }
  else if (phase2_.divU().valid())
  {
    tdgdt =
    (
      -alpha1.dimensionedInternalField()
      *phase2_.divU()().dimensionedInternalField()
    );
  }
  alpha1.correctBoundaryConditions();
  surfaceScalarField alpha1f{fvc::interpolate(max(alpha1, scalar(0)))};
  surfaceScalarField phic{"phic", phi};
  surfaceScalarField phir{"phir", phi1 - phi2};
  tmp<surfaceScalarField> alphaDbyA;
  if (notNull(phase1_.DbyA()) && notNull(phase2_.DbyA()))
  {
    surfaceScalarField DbyA(phase1_.DbyA() + phase2_.DbyA());
    alphaDbyA =
      fvc::interpolate(max(alpha1, scalar(0)))
      *fvc::interpolate(max(alpha2, scalar(0)))
      *DbyA;
    phir += DbyA*fvc::snGrad(alpha1, "bounded")*mesh_.magSf();
  }
  for (int acorr=0; acorr<nAlphaCorr; acorr++)
  {
    volScalarField::DimensionedInternalField Sp
    {
      {
        "Sp",
        runTime.timeName(),
        mesh_
      },
      mesh_,
      {"Sp", dimless/dimTime, 0.0}
    };
    volScalarField::DimensionedInternalField Su
    {
      {
        "Su",
        runTime.timeName(),
        mesh_
      },
      // Divergence term is handled explicitly to be
      // consistent with the explicit transport solution
      fvc::div(phi)*min(alpha1, scalar(1))
    };
    if (tdgdt.valid())
    {
      scalarField& dgdt = tdgdt();
      FOR_ALL(dgdt, celli)
      {
        if (dgdt[celli] > 0.0)
        {
          Sp[celli] -= dgdt[celli]/max(1.0 - alpha1[celli], 1e-4);
          Su[celli] += dgdt[celli]/max(1.0 - alpha1[celli], 1e-4);
        }
        else if (dgdt[celli] < 0.0)
        {
          Sp[celli] += dgdt[celli]/max(alpha1[celli], 1e-4);
        }
      }
    }
    surfaceScalarField alphaPhic1
    {
      fvc::flux
      (
        phic,
        alpha1,
        alphaScheme
      )
      + fvc::flux
      (
       -fvc::flux(-phir, scalar(1) - alpha1, alpharScheme),
        alpha1,
        alpharScheme
      )
    };
    // Ensure that the flux at inflow BCs is preserved
    FOR_ALL(alphaPhic1.boundaryField(), patchi)
    {
      fvsPatchScalarField& alphaPhic1p = alphaPhic1.boundaryField()[patchi];
      if (!alphaPhic1p.coupled())
      {
        const scalarField& phi1p = phi1.boundaryField()[patchi];
        const scalarField& alpha1p = alpha1.boundaryField()[patchi];
        FOR_ALL(alphaPhic1p, facei)
        {
          if (phi1p[facei] < 0)
          {
            alphaPhic1p[facei] = alpha1p[facei]*phi1p[facei];
          }
        }
      }
    }
    if (nAlphaSubCycles > 1)
    {
      tmp<volScalarField> trSubDeltaT;
      if (LTS)
      {
        trSubDeltaT =
          fv::localEulerDdt::localRSubDeltaT(mesh_, nAlphaSubCycles);
      }
      for
      (
        subCycle<volScalarField> alphaSubCycle{alpha1, nAlphaSubCycles};
        !(++alphaSubCycle).end();
      )
      {
        surfaceScalarField alphaPhic10(alphaPhic1);
        MULES::explicitSolve
        (
          geometricOneField(),
          alpha1,
          phi,
          alphaPhic10,
          (alphaSubCycle.index()*Sp)(),
          (Su - (alphaSubCycle.index() - 1)*Sp*alpha1)(),
          phase1_.alphaMax(),
          0
        );
        if (alphaSubCycle.index() == 1)
        {
          phase1_.alphaPhi() = alphaPhic10;
        }
        else
        {
          phase1_.alphaPhi() += alphaPhic10;
        }
      }
      phase1_.alphaPhi() /= nAlphaSubCycles;
    }
    else
    {
      MULES::explicitSolve
      (
        geometricOneField(),
        alpha1,
        phi,
        alphaPhic1,
        Sp,
        Su,
        phase1_.alphaMax(),
        0
      );
      phase1_.alphaPhi() = alphaPhic1;
    }
    if (alphaDbyA.valid())
    {
      fvScalarMatrix alpha1Eqn
      {
        fvm::ddt(alpha1) - fvc::ddt(alpha1)
        - fvm::laplacian(alphaDbyA, alpha1, "bounded")
      };
      alpha1Eqn.relax();
      alpha1Eqn.solve();
      phase1_.alphaPhi() += alpha1Eqn.flux();
    }
    phase1_.alphaRhoPhi() =
      fvc::interpolate(phase1_.rho())*phase1_.alphaPhi();
    phase2_.alphaPhi() = phi - phase1_.alphaPhi();
    alpha2 = scalar(1) - alpha1;
    phase2_.alphaRhoPhi() =
      fvc::interpolate(phase2_.rho())*phase2_.alphaPhi();
    Info << alpha1.name() << " volume fraction = "
      << alpha1.weightedAverage(mesh_.V()).value()
      << "  Min(alpha1) = " << min(alpha1).value()
      << "  Max(alpha1) = " << max(alpha1).value()
      << endl;
  }
}
