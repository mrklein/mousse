// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interface_composition_phase_change_phase_system.hpp"
#include "interface_composition_model.hpp"
// Constructors 
template<class BasePhaseSystem>
mousse::InterfaceCompositionPhaseChangePhaseSystem<BasePhaseSystem>::
InterfaceCompositionPhaseChangePhaseSystem
(
  const fvMesh& mesh
)
:
  HeatAndMassTransferPhaseSystem<BasePhaseSystem>{mesh}
{
  this->generatePairsAndSubModels
  (
    "interfaceComposition",
    interfaceCompositionModels_
  );
}
// Destructor 
template<class BasePhaseSystem>
mousse::InterfaceCompositionPhaseChangePhaseSystem<BasePhaseSystem>::
~InterfaceCompositionPhaseChangePhaseSystem()
{}
// Member Functions 
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::massTransferTable>
mousse::InterfaceCompositionPhaseChangePhaseSystem<BasePhaseSystem>::
massTransfer() const
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
  // Reset the interfacial mass flow rates
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
    *this->dmdt_[pair] = *this->dmdtExplicit_[pair];
    *this->dmdtExplicit_[pair] = {"zero", dimDensity/dimTime, 0};
  }
  // Sum up the contribution from each interface composition model
  FOR_ALL_CONST_ITER
  (
    interfaceCompositionModelTable,
    interfaceCompositionModels_,
    interfaceCompositionModelIter
  )
  {
    const interfaceCompositionModel& compositionModel =
      interfaceCompositionModelIter();
    const phasePair& pair =
      this->phasePairs_[interfaceCompositionModelIter.key()];
    const phaseModel& phase = pair.phase1();
    const phaseModel& otherPhase = pair.phase2();
    const phasePairKey key(phase.name(), otherPhase.name());
    const volScalarField& Tf = *this->Tf_[key];
    volScalarField& dmdtExplicit = *this->dmdtExplicit_[key];
    volScalarField& dmdt = *this->dmdt_[key];
    scalar dmdtSign
    {
      static_cast<double>(Pair<word>::compare(this->dmdt_.find(key).key(), key))
    };
    const volScalarField K
    {
      this->massTransferModels_[key][phase.name()]->K()
    };
    FOR_ALL_CONST_ITER
    (
      hashedWordList,
      compositionModel.species(),
      memberIter
    )
    {
      const word& member = *memberIter;
      const word name
      {
        IOobject::groupName(member, phase.name())
      };
      const word otherName
      {
        IOobject::groupName(member, otherPhase.name())
      };
      const volScalarField KD
      {
        K*compositionModel.D(member)
      };
      const volScalarField Yf
      {
        compositionModel.Yf(member, Tf)
      };
      // Implicit transport through the phase
      *eqns[name] +=
        phase.rho()*KD*Yf
        - fvm::Sp(phase.rho()*KD, eqns[name]->psi());
      // Sum the mass transfer rate
      dmdtExplicit += dmdtSign*phase.rho()*KD*Yf;
      dmdt -= dmdtSign*phase.rho()*KD*eqns[name]->psi();
      // Explicit transport out of the other phase
      if (eqns.found(otherName))
      {
        *eqns[otherName] -= otherPhase.rho()*KD*compositionModel.dY(member, Tf);
      }
    }
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
void mousse::InterfaceCompositionPhaseChangePhaseSystem<BasePhaseSystem>::
correctThermo()
{
  BasePhaseSystem::correctThermo();
  // This loop solves for the interface temperatures, Tf, and updates the
  // interface composition models.
  //
  // The rate of heat transfer to the interface must equal the latent heat
  // consumed at the interface, i.e.:
  //
  // H1*(T1 - Tf) + H2*(T2 - Tf) == mDotL
  //                             == K*rho*(Yfi - Yi)*Li
  //
  // Yfi is likely to be a strong non-linear (typically exponential) function
  // of Tf, so the solution for the temperature is newton-accelerated
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
    const phasePairKey key12{pair.first(), pair.second(), true};
    const phasePairKey key21{pair.second(), pair.first(), true};
    volScalarField H1{this->heatTransferModels_[pair][pair.first()]->K()};
    volScalarField H2{this->heatTransferModels_[pair][pair.second()]->K()};
    dimensionedScalar HSmall{"small", heatTransferModel::dimK, SMALL};
    volScalarField mDotL
    {
      {
        "mDotL",
        this->mesh().time().timeName(),
        this->mesh()
      },
      this->mesh(),
      {"zero", dimEnergy/dimVolume/dimTime, 0}
    };
    volScalarField mDotLPrime
    {
      {
        "mDotLPrime",
        this->mesh().time().timeName(),
        this->mesh()
      },
      this->mesh(),
      {"zero", mDotL.dimensions()/dimTemperature, 0}
    };
    volScalarField& Tf = *this->Tf_[pair];
    // Add latent heats from forward and backward models
    if (this->interfaceCompositionModels_.found(key12))
    {
      this->interfaceCompositionModels_[key12]->addMDotL
      (
        this->massTransferModels_[pair][pair.first()]->K(),
        Tf,
        mDotL,
        mDotLPrime
      );
    }
    if (this->interfaceCompositionModels_.found(key21))
    {
      this->interfaceCompositionModels_[key21]->addMDotL
      (
        this->massTransferModels_[pair][pair.second()]->K(),
        Tf,
        mDotL,
        mDotLPrime
      );
    }
    // Update the interface temperature by applying one step of newton's
    // method to the interface relation
    Tf -= (H1*(Tf - pair.phase1().thermo().T())
           + H2*(Tf - pair.phase2().thermo().T())
           + mDotL)/(max(H1 + H2 + mDotLPrime, HSmall));
    Tf.correctBoundaryConditions();
    Info << "Tf." << pair.name()
      << ": min = " << min(Tf.internalField())
      << ", mean = " << average(Tf.internalField())
      << ", max = " << max(Tf.internalField())
      << endl;
    // Update the interface compositions
    if (this->interfaceCompositionModels_.found(key12))
    {
      this->interfaceCompositionModels_[key12]->update(Tf);
    }
    if (this->interfaceCompositionModels_.found(key21))
    {
      this->interfaceCompositionModels_[key21]->update(Tf);
    }
  }
}
template<class BasePhaseSystem>
bool mousse::InterfaceCompositionPhaseChangePhaseSystem<BasePhaseSystem>::read()
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
