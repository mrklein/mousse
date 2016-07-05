// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_phase_system.hpp"
#include "blended_interfacial_model.hpp"
#include "heat_transfer_model.hpp"
#include "hash_ptr_table.hpp"
#include "fvc_div.hpp"
#include "fvm_sup.hpp"
#include "fv_matrix.hpp"
// Constructors 
template<class BasePhaseSystem>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::HeatTransferPhaseSystem
(
  const fvMesh& mesh
)
:
  BasePhaseSystem{mesh}
{
  this->generatePairsAndSubModels("heatTransfer", heatTransferModels_);
}
// Destructor 
template<class BasePhaseSystem>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::~HeatTransferPhaseSystem()
{}
// Member Functions 
template<class BasePhaseSystem>
bool mousse::HeatTransferPhaseSystem<BasePhaseSystem>::transfersMass
(
  const phaseModel& /*phase*/
) const
{
  return false;
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::dmdt
(
  const phasePairKey& key
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        IOobject::groupName
        (
          "dmdt",
          this->phasePairs_[key]->name()
        ),
        this->mesh().time().timeName(),
        this->mesh().time()
      },
      this->mesh(),
      {"zero", dimDensity/dimTime, 0}
    }
  };
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::dmdt
(
  const mousse::phaseModel& /*phase*/
) const
{
  return tmp<volScalarField>{NULL};
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::heatTransferTable>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::heatTransfer() const
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
  FOR_ALL_CONST_ITER
  (
    heatTransferModelTable,
    heatTransferModels_,
    heatTransferModelIter
  )
  {
    const volScalarField K{heatTransferModelIter()->K()};
    const phasePair& pair = this->phasePairs_[heatTransferModelIter.key()];
    const phaseModel* phase = &pair.phase1();
    const phaseModel* otherPhase = &pair.phase2();
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      const volScalarField& he = phase->thermo().he();
      volScalarField Cpv(phase->thermo().Cpv());
      *eqns[phase->name()] +=
        K*(otherPhase->thermo().T() - phase->thermo().T() + he/Cpv)
        - fvm::Sp(K/Cpv, he);
      Swap(phase, otherPhase);
    }
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::massTransferTable>
mousse::HeatTransferPhaseSystem<BasePhaseSystem>::massTransfer() const
{
  autoPtr<phaseSystem::massTransferTable> eqnsPtr
  {
    new phaseSystem::massTransferTable{}
  };
  return eqnsPtr;
}
template<class BasePhaseSystem>
bool mousse::HeatTransferPhaseSystem<BasePhaseSystem>::read()
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
