// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_phase_change_phase_system.hpp"
#include "fv_cfd.hpp"
#include "alphat_phase_change_wall_function_fv_patch_scalar_field.hpp"
// Constructors 
template<class BasePhaseSystem>
mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::
ThermalPhaseChangePhaseSystem
(
  const fvMesh& mesh
)
:
  HeatAndMassTransferPhaseSystem<BasePhaseSystem>{mesh},
  volatile_{this->lookup("volatile")},
  saturationModel_{saturationModel::New(this->subDict("saturationModel"))},
  massTransfer_{this->lookup("massTransfer")}
{
  FOR_ALL_CONST_ITER
  (
    phaseSystem::phasePairTable,
    this->phasePairs_,
    phasePairIter
  )
  {
    const phasePair& pair = phasePairIter();
    if (pair.ordered())
    {
      continue;
    }
    // Initially assume no mass transfer
    iDmdt_.insert
    (
      pair,
      new volScalarField
      {
        {
          IOobject::groupName("iDmdt", pair.name()),
          this->mesh().time().timeName(),
          this->mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", dimDensity/dimTime, 0}
      }
    );
    // Initially assume no mass transfer
    wDmdt_.insert
    (
      pair,
      new volScalarField
      {
        {
          IOobject::groupName("wDmdt", pair.name()),
          this->mesh().time().timeName(),
          this->mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", dimDensity/dimTime, 0}
      }
    );
  }
}
// Destructor 
template<class BasePhaseSystem>
mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::
~ThermalPhaseChangePhaseSystem()
{}
// Member Functions 
template<class BasePhaseSystem>
const mousse::saturationModel&
mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::saturation() const
{
  return saturationModel_();
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::massTransferTable>
mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::massTransfer() const
{
  // Create a mass transfer matrix for each species of each phase
  autoPtr<phaseSystem::massTransferTable> eqnsPtr
  {
    new phaseSystem::massTransferTable()
  };
  phaseSystem::massTransferTable& eqns = eqnsPtr();
  FOR_ALL(this->phaseModels_, phasei)
  {
    const phaseModel& phase = this->phaseModels_[phasei];
    const PtrList<volScalarField>& Yi = phase.Y();
    FOR_ALL(Yi, i)
    {
      eqns.insert
      (
        Yi[i].name(),
        new fvScalarMatrix{Yi[i], dimMass/dimTime}
      );
    }
  }
  FOR_ALL_CONST_ITER
  (
    phaseSystem::phasePairTable,
    this->phasePairs_,
    phasePairIter
  )
  {
    const phasePair& pair = phasePairIter();
    if (pair.ordered())
    {
      continue;
    }
    const phaseModel& phase = pair.phase1();
    const phaseModel& otherPhase = pair.phase2();
    const word name
    {
      IOobject::groupName(volatile_, phase.name())
    };
    const word otherName
    {
      IOobject::groupName(volatile_, otherPhase.name())
    };
    const volScalarField dmdt{this->dmdt(pair)};
    const volScalarField dmdt12{posPart(dmdt)};
    const volScalarField dmdt21{negPart(dmdt)};
    *eqns[name] += fvm::Sp(dmdt21, eqns[name]->psi()) - dmdt21;
    *eqns[otherName] += dmdt12 - fvm::Sp(dmdt12, eqns[otherName]->psi());
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
void mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::correctThermo()
{
  typedef compressible::alphatPhaseChangeWallFunctionFvPatchScalarField
    alphatPhaseChangeWallFunction;
  BasePhaseSystem::correctThermo();
  FOR_ALL_CONST_ITER
  (
    phaseSystem::phasePairTable,
    this->phasePairs_,
    phasePairIter
  )
  {
    const phasePair& pair = phasePairIter();
    if (pair.ordered())
    {
      continue;
    }
    const phaseModel& phase1 = pair.phase1();
    const phaseModel& phase2 = pair.phase2();
    const volScalarField& T1 = phase1.thermo().T();
    const volScalarField& T2 = phase2.thermo().T();
    const volScalarField& he1 = phase1.thermo().he();
    const volScalarField& he2 = phase2.thermo().he();
    volScalarField& dmdt = *this->dmdt_[pair];
    volScalarField& iDmdt = *this->iDmdt_[pair];
    volScalarField& wDmdt = *this->wDmdt_[pair];
    volScalarField& Tf = *this->Tf_[pair];
    volScalarField hef1{phase1.thermo().he(phase1.thermo().p(), Tf)};
    volScalarField hef2{phase2.thermo().he(phase2.thermo().p(), Tf)};
    if (massTransfer_ )
    {
      volScalarField H1
      {
        this->heatTransferModels_[pair][pair.first()]->K(0)
      };
      volScalarField H2
      {
        this->heatTransferModels_[pair][pair.second()]->K(0)
      };
      Tf = saturationModel_->Tsat(phase1.thermo().p());
      scalar iDmdtRelax{this->mesh().fieldRelaxationFactor("iDmdt")};
      iDmdt = (1 - iDmdtRelax)*iDmdt
        + iDmdtRelax*(H1*(Tf - T1) + H2*(Tf - T2))
        /min((pos(iDmdt)*he2 + neg(iDmdt)*hef2)
             - (neg(iDmdt)*he1 + pos(iDmdt)*hef1),
             0.3*mag(hef2 - hef1));
      Info << "iDmdt." << pair.name()
        << ": min = " << min(iDmdt.internalField())
        << ", mean = " << average(iDmdt.internalField())
        << ", max = " << max(iDmdt.internalField())
        << ", integral = " << fvc::domainIntegrate(iDmdt).value()
        << endl;
    }
    else
    {
      iDmdt == dimensionedScalar{"0", dmdt.dimensions(), 0};
    }
    volScalarField H1{this->heatTransferModels_[pair][pair.first()]->K()};
    volScalarField H2{this->heatTransferModels_[pair][pair.second()]->K()};
    // Limit the H[12] boundary field to avoid /0
    const scalar HLimit = 1e-4;
    H1.boundaryField() =
      max(H1.boundaryField(), phase1.boundaryField()*HLimit);
    H2.boundaryField() =
      max(H2.boundaryField(), phase2.boundaryField()*HLimit);
    volScalarField mDotL
    {
      iDmdt*
      (
        (pos(iDmdt)*he2 + neg(iDmdt)*hef2) - (neg(iDmdt)*he1 + pos(iDmdt)*hef1)
      )
    };
    Tf = (H1*T1 + H2*T2 + mDotL)/(H1 + H2);
    Info << "Tf." << pair.name()
      << ": min = " << min(Tf.internalField())
      << ", mean = " << average(Tf.internalField())
      << ", max = " << max(Tf.internalField())
      << endl;
    // Accumulate dmdt contributions from boundaries
    if
    (
      phase2.mesh().foundObject<volScalarField>
      (
        "alphat." +  phase2.name()
      )
    )
    {
      scalar wDmdtRelax{this->mesh().fieldRelaxationFactor("wDmdt")};
      wDmdt *= (1 - wDmdtRelax);
      const volScalarField& alphat =
        phase2.mesh().lookupObject<volScalarField>
        (
          "alphat." +  phase2.name()
        );
      const fvPatchList& patches = this->mesh().boundary();
      FOR_ALL(patches, patchi)
      {
        const fvPatch& currPatch = patches[patchi];
        if
        (
          isA<alphatPhaseChangeWallFunction>
          (
            alphat.boundaryField()[patchi]
          )
        )
        {
          const scalarField& patchDmdt =
            refCast<const alphatPhaseChangeWallFunction>
            (
              alphat.boundaryField()[patchi]
            ).dmdt();
          FOR_ALL(patchDmdt,facei)
          {
            label faceCelli = currPatch.faceCells()[facei];
            wDmdt[faceCelli] += wDmdtRelax*patchDmdt[facei];
          }
        }
      }
      Info << "wDmdt." << pair.name()
        << ": min = " << min(wDmdt.internalField())
        << ", mean = " << average(wDmdt.internalField())
        << ", max = " << max(wDmdt.internalField())
        << ", integral = " << fvc::domainIntegrate(wDmdt).value()
        << endl;
    }
    dmdt = wDmdt + iDmdt;
    Info << "dmdt." << pair.name()
      << ": min = " << min(dmdt.internalField())
      << ", mean = " << average(dmdt.internalField())
      << ", max = " << max(dmdt.internalField())
      << ", integral = " << fvc::domainIntegrate(dmdt).value()
      << endl;
  }
}
template<class BasePhaseSystem>
bool mousse::ThermalPhaseChangePhaseSystem<BasePhaseSystem>::read()
{
  if (BasePhaseSystem::read())
  {
    bool readOK = true;
    // Models ...
    return readOK;
  }
  else
  {
    return false;
  }
}
