// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_and_mass_transfer_phase_system.hpp"
#include "blended_interfacial_model.hpp"
#include "heat_transfer_model.hpp"
#include "mass_transfer_model.hpp"
#include "hash_ptr_table.hpp"
#include "fvc_div.hpp"
#include "fvm_sup.hpp"
#include "fv_matrix.hpp"
// Constructors 
template<class BasePhaseSystem>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::
HeatAndMassTransferPhaseSystem
(
  const fvMesh& mesh
)
:
  BasePhaseSystem{mesh}
{
  this->generatePairsAndSubModels
  (
    "heatTransfer",
    heatTransferModels_
  );
  this->generatePairsAndSubModels
  (
    "massTransfer",
    massTransferModels_
  );
  FOR_ALL_CONST_ITER
  (
    phaseSystem::phasePairTable,
    this->phasePairs_,
    phasePairIter
  )
  {
    const phasePair& pair(phasePairIter());
    if (pair.ordered())
    {
      continue;
    }
    // Initialy assume no mass transfer
    dmdt_.insert
    (
      pair,
      new volScalarField
      {
        {
          IOobject::groupName("dmdt", pair.name()),
          this->mesh().time().timeName(),
          this->mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", dimDensity/dimTime, 0}
      }
    );
    dmdtExplicit_.insert
    (
      pair,
      new volScalarField
      {
        {
          IOobject::groupName("dmdtExplicit", pair.name()),
          this->mesh().time().timeName(),
          this->mesh()
        },
        this->mesh(),
        {"zero", dimDensity/dimTime, 0}
      }
    );
    volScalarField H1{heatTransferModels_[pair][pair.first()]->K()};
    volScalarField H2{heatTransferModels_[pair][pair.second()]->K()};
    Tf_.insert
    (
      pair,
      new volScalarField
      {
        {
          IOobject::groupName("Tf", pair.name()),
          this->mesh().time().timeName(),
          this->mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        (H1*pair.phase1().thermo().T() + H2*pair.phase2().thermo().T())
        /max(H1 + H2,
             dimensionedScalar("small", heatTransferModel::dimK, SMALL)),
        zeroGradientFvPatchScalarField::typeName
      }
    );
    Tf_[pair]->correctBoundaryConditions();
  }
}
// Destructor 
template<class BasePhaseSystem>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::
~HeatAndMassTransferPhaseSystem()
{}
// Member Functions 
template<class BasePhaseSystem>
bool mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::transfersMass
(
  const phaseModel& /*phase*/
) const
{
  return true;
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::dmdt
(
  const phasePairKey& key
) const
{
  const scalar dmdtSign
  {
    static_cast<double>(Pair<word>::compare(dmdt_.find(key).key(), key))
  };
  return dmdtSign*(*dmdt_[key]);
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::dmdt
(
  const mousse::phaseModel& phase
) const
{
  tmp<volScalarField> tdmdt
  {
    new volScalarField
    {
      {
        IOobject::groupName("dmdt", phase.name()),
        this->mesh_.time().timeName(),
        this->mesh_
      },
      this->mesh_,
      {"zero", dimDensity/dimTime, 0}
    }
  };
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
    const phaseModel* phase1 = &pair.phase1();
    const phaseModel* phase2 = &pair.phase2();
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      if (phase1 == &phase)
      {
        tdmdt() += this->dmdt(pair);
      }
      Swap(phase1, phase2);
    }
  }
  return tdmdt;
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::momentumTransferTable>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::momentumTransfer() const
{
  autoPtr<phaseSystem::momentumTransferTable>
    eqnsPtr(BasePhaseSystem::momentumTransfer());
  phaseSystem::momentumTransferTable& eqns = eqnsPtr();
  // Source term due to mass trasfer
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
    const volVectorField& U1 = pair.phase1().U();
    const volVectorField& U2 = pair.phase2().U();
    const volScalarField dmdt{this->dmdt(pair)};
    const volScalarField dmdt21{posPart(dmdt)};
    const volScalarField dmdt12{negPart(dmdt)};
    *eqns[pair.phase1().name()] += dmdt21*U2 - fvm::Sp(dmdt21, U1);
    *eqns[pair.phase2().name()] -= dmdt12*U1 - fvm::Sp(dmdt12, U2);
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::heatTransferTable>
mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::heatTransfer() const
{
  autoPtr<phaseSystem::heatTransferTable> eqnsPtr
  {
    new phaseSystem::heatTransferTable()
  };
  phaseSystem::heatTransferTable& eqns = eqnsPtr();
  FOR_ALL(this->phaseModels_, phasei)
  {
    const phaseModel& phase = this->phaseModels_[phasei];
    eqns.insert
    (
      phase.name(),
      new fvScalarMatrix{phase.thermo().he(), dimEnergy/dimTime}
    );
  }
  // Heat transfer with the interface
  FOR_ALL_CONST_ITER
  (
    heatTransferModelTable,
    heatTransferModels_,
    heatTransferModelIter
  )
  {
    const phasePair& pair
    (
      this->phasePairs_[heatTransferModelIter.key()]
    );
    const phaseModel* phase = &pair.phase1();
    const phaseModel* otherPhase = &pair.phase2();
    const volScalarField& Tf = *Tf_[pair];
    const volScalarField K1
    {
      heatTransferModelIter()[pair.first()]->K()
    };
    const volScalarField K2
    {
      heatTransferModelIter()[pair.second()]->K()
    };
    const volScalarField KEff
    {
      K1*K2/max(K1 + K2,
                dimensionedScalar("small", heatTransferModel::dimK, SMALL))
    };
    const volScalarField* K = &K1;
    const volScalarField* otherK = &K2;
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      const volScalarField& he = phase->thermo().he();
      volScalarField Cpv(phase->thermo().Cpv());
      *eqns[phase->name()] +=
        (*K)*(Tf - phase->thermo().T()) + KEff/Cpv*he - fvm::Sp(KEff/Cpv, he);
      Swap(phase, otherPhase);
      Swap(K, otherK);
    }
  }
  // Source term due to mass transfer
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
    const volScalarField& he1 = phase1.thermo().he();
    const volScalarField& he2 = phase2.thermo().he();
    const volScalarField& K1 = phase1.K();
    const volScalarField& K2 = phase2.K();
    const volScalarField dmdt{this->dmdt(pair)};
    const volScalarField dmdt21{posPart(dmdt)};
    const volScalarField dmdt12{negPart(dmdt)};
    const volScalarField& Tf = *Tf_[pair];
    *eqns[phase1.name()] +=
      dmdt21*(phase1.thermo().he(phase1.thermo().p(), Tf))
      - fvm::Sp(dmdt21, he1)
      + dmdt21*(K2 - K1);
    *eqns[phase2.name()] -=
      dmdt12*(phase2.thermo().he(phase2.thermo().p(), Tf))
      - fvm::Sp(dmdt12, he2)
      + dmdt12*(K1 - K2);
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
bool mousse::HeatAndMassTransferPhaseSystem<BasePhaseSystem>::read()
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
