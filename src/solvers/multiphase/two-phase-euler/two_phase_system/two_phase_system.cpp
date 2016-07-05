// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "two_phase_system.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "blended_interfacial_model.hpp"
#include "virtual_mass_model.hpp"
#include "heat_transfer_model.hpp"
#include "lift_model.hpp"
#include "wall_lubrication_model.hpp"
#include "turbulent_dispersion_model.hpp"
#include "fv_matrix.hpp"
#include "surface_interpolate.hpp"
#include "mules.hpp"
#include "sub_cycle.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
#include "fvc_sn_grad.hpp"
#include "fvc_flux.hpp"
#include "fvc_curl.hpp"
#include "fvm_ddt.hpp"
#include "fvm_laplacian.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
#include "blending_method.hpp"
#include "hash_ptr_table.hpp"
// Constructors 
mousse::twoPhaseSystem::twoPhaseSystem
(
  const fvMesh& mesh,
  const dimensionedVector& g
)
:
  IOdictionary
  {
    {
      "phaseProperties",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  },
  mesh_{mesh},
  phase1_
  {
    *this,
    *this,
    wordList{lookup("phases")}[0]
  },
  phase2_
  {
    *this,
    *this,
    wordList{lookup("phases")}[1]
  },
  phi_
  {
    {
      "phi",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    this->calcPhi()
  },
  dgdt_
  {
    {
      "dgdt",
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh,
    {"dgdt", dimless/dimTime, 0}
  }
{
  phase2_.volScalarField::operator=(scalar(1) - phase1_);
  // Blending
  FOR_ALL_CONST_ITER(dictionary, subDict("blending"), iter)
  {
    blendingMethods_.insert
    (
      iter().dict().dictName(),
      blendingMethod::New
      (
        iter().dict(),
        wordList{lookup("phases")}
      )
    );
  }
  // Pairs
  phasePair::scalarTable sigmaTable{lookup("sigma")};
  phasePair::dictTable aspectRatioTable{lookup("aspectRatio")};
  pair_.set
  (
    new phasePair
    (
      phase1_,
      phase2_,
      g,
      sigmaTable
    )
  );
  pair1In2_.set
  (
    new orderedPhasePair
    (
      phase1_,
      phase2_,
      g,
      sigmaTable,
      aspectRatioTable
    )
  );
  pair2In1_.set
  (
    new orderedPhasePair
    (
      phase2_,
      phase1_,
      g,
      sigmaTable,
      aspectRatioTable
    )
  );
  // Models
  drag_.set
  (
    new BlendedInterfacialModel<dragModel>
    {
      lookup("drag"),
      (
        blendingMethods_.found("drag")
        ? blendingMethods_["drag"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_,
      false // Do not zero drag coefficent at fixed-flux BCs
    }
  );
  virtualMass_.set
  (
    new BlendedInterfacialModel<virtualMassModel>
    {
      lookup("virtualMass"),
      (
        blendingMethods_.found("virtualMass")
        ? blendingMethods_["virtualMass"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_
    }
  );
  heatTransfer_.set
  (
    new BlendedInterfacialModel<heatTransferModel>
    {
      lookup("heatTransfer"),
      (
        blendingMethods_.found("heatTransfer")
        ? blendingMethods_["heatTransfer"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_
    }
  );
  lift_.set
  (
    new BlendedInterfacialModel<liftModel>
    {
      lookup("lift"),
      (
        blendingMethods_.found("lift")
        ? blendingMethods_["lift"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_
    }
  );
  wallLubrication_.set
  (
    new BlendedInterfacialModel<wallLubricationModel>
    {
      lookup("wallLubrication"),
      (
        blendingMethods_.found("wallLubrication")
        ? blendingMethods_["wallLubrication"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_
    }
  );
  turbulentDispersion_.set
  (
    new BlendedInterfacialModel<turbulentDispersionModel>
    {
      lookup("turbulentDispersion"),
      (
        blendingMethods_.found("turbulentDispersion")
        ? blendingMethods_["turbulentDispersion"]
        : blendingMethods_["default"]
      ),
      pair_,
      pair1In2_,
      pair2In1_
    }
  );
}
// Destructor 
mousse::twoPhaseSystem::~twoPhaseSystem()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::twoPhaseSystem::rho() const
{
  return phase1_*phase1_.thermo().rho() + phase2_*phase2_.thermo().rho();
}
mousse::tmp<mousse::volVectorField> mousse::twoPhaseSystem::U() const
{
  return phase1_*phase1_.U() + phase2_*phase2_.U();
}
mousse::tmp<mousse::surfaceScalarField> mousse::twoPhaseSystem::calcPhi() const
{
  return
    fvc::interpolate(phase1_)*phase1_.phi()
    + fvc::interpolate(phase2_)*phase2_.phi();
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseSystem::Kd() const
{
  return drag_->K();
}
mousse::tmp<mousse::surfaceScalarField> mousse::twoPhaseSystem::Kdf() const
{
  return drag_->Kf();
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseSystem::Vm() const
{
  return virtualMass_->K();
}
mousse::tmp<mousse::surfaceScalarField> mousse::twoPhaseSystem::Vmf() const
{
  return virtualMass_->Kf();
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseSystem::Kh() const
{
  return heatTransfer_->K();
}
mousse::tmp<mousse::volVectorField> mousse::twoPhaseSystem::F() const
{
  return lift_->F<vector>() + wallLubrication_->F<vector>();
}
mousse::tmp<mousse::surfaceScalarField> mousse::twoPhaseSystem::Ff() const
{
  return lift_->Ff() + wallLubrication_->Ff();
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseSystem::D() const
{
  return turbulentDispersion_->D();
}
void mousse::twoPhaseSystem::solve()
{
  const Time& runTime = mesh_.time();
  volScalarField& alpha1 = phase1_;
  volScalarField& alpha2 = phase2_;
  const surfaceScalarField& phi1 = phase1_.phi();
  const surfaceScalarField& phi2 = phase2_.phi();
  const dictionary& alphaControls = mesh_.solverDict
  (
    alpha1.name()
  );
  label nAlphaSubCycles{readLabel(alphaControls.lookup("nAlphaSubCycles"))};
  label nAlphaCorr{readLabel(alphaControls.lookup("nAlphaCorr"))};
  word alphaScheme{"div(phi," + alpha1.name() + ')'};
  word alpharScheme{"div(phir," + alpha1.name() + ')'};
  alpha1.correctBoundaryConditions();
  surfaceScalarField phic{"phic", phi_};
  surfaceScalarField phir{"phir", phi1 - phi2};
  tmp<surfaceScalarField> alpha1alpha2f;
  if (pPrimeByA_.valid())
  {
    alpha1alpha2f =
      fvc::interpolate(max(alpha1, scalar(0)))
      *fvc::interpolate(max(alpha2, scalar(0)));
    surfaceScalarField phiP
    {
      pPrimeByA_()*fvc::snGrad(alpha1, "bounded")*mesh_.magSf()
    };
    phir += phiP;
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
      {"Sp", dgdt_.dimensions(), 0.0}
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
      fvc::div(phi_)*min(alpha1, scalar(1))
    };
    FOR_ALL(dgdt_, celli)
    {
      if (dgdt_[celli] > 0.0)
      {
        Sp[celli] -= dgdt_[celli]/max(1.0 - alpha1[celli], 1e-4);
        Su[celli] += dgdt_[celli]/max(1.0 - alpha1[celli], 1e-4);
      }
      else if (dgdt_[celli] < 0.0)
      {
        Sp[celli] += dgdt_[celli]/max(alpha1[celli], 1e-4);
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
      for
      (
        subCycle<volScalarField> alphaSubCycle{alpha1, nAlphaSubCycles};
        !(++alphaSubCycle).end();
      )
      {
        surfaceScalarField alphaPhic10{alphaPhic1};
        MULES::explicitSolve
        (
          geometricOneField(),
          alpha1,
          phi_,
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
        phi_,
        alphaPhic1,
        Sp,
        Su,
        phase1_.alphaMax(),
        0
      );
      phase1_.alphaPhi() = alphaPhic1;
    }
    if (pPrimeByA_.valid())
    {
      fvScalarMatrix alpha1Eqn
      {
        fvm::ddt(alpha1) - fvc::ddt(alpha1)
        - fvm::laplacian(alpha1alpha2f*pPrimeByA_(), alpha1, "bounded")
      };
      alpha1Eqn.relax();
      alpha1Eqn.solve();
      phase1_.alphaPhi() += alpha1Eqn.flux();
    }
    phase1_.alphaRhoPhi() = fvc::interpolate(phase1_.rho())*phase1_.alphaPhi();
    phase2_.alphaPhi() = phi_ - phase1_.alphaPhi();
    alpha2 = scalar(1) - alpha1;
    phase2_.alphaRhoPhi() =
      fvc::interpolate(phase2_.rho())*phase2_.alphaPhi();
    Info << alpha1.name() << " volume fraction = "
      << alpha1.weightedAverage(mesh_.V()).value()
      << "  Min(" << alpha1.name() << ") = " << min(alpha1).value()
      << "  Max(" << alpha1.name() << ") = " << max(alpha1).value()
      << endl;
  }
}
void mousse::twoPhaseSystem::correct()
{
  phase1_.correct();
  phase2_.correct();
}
void mousse::twoPhaseSystem::correctTurbulence()
{
  phase1_.turbulence().correct();
  phase2_.turbulence().correct();
}
bool mousse::twoPhaseSystem::read()
{
  if (regIOobject::read())
  {
    bool readOK = true;
    readOK &= phase1_.read(*this);
    readOK &= phase2_.read(*this);
    // models ...
    return readOK;
  }
  else
  {
    return false;
  }
}
const mousse::dragModel& mousse::twoPhaseSystem::drag(const phaseModel& phase) const
{
  return drag_->phaseModel(phase);
}
const mousse::virtualMassModel&
mousse::twoPhaseSystem::virtualMass(const phaseModel& phase) const
{
  return virtualMass_->phaseModel(phase);
}
const mousse::dimensionedScalar& mousse::twoPhaseSystem::sigma() const
{
  return pair_->sigma();
}
