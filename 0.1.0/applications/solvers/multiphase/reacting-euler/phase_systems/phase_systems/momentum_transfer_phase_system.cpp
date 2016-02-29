// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "momentum_transfer_phase_system.hpp"
#include "blended_interfacial_model.hpp"
#include "drag_model.hpp"
#include "virtual_mass_model.hpp"
#include "lift_model.hpp"
#include "wall_lubrication_model.hpp"
#include "turbulent_dispersion_model.hpp"
#include "hash_ptr_table.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "fvc_div.hpp"
#include "fvc_sn_grad.hpp"
#include "fv_matrix.hpp"
// Constructors 
template<class BasePhaseSystem>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::
MomentumTransferPhaseSystem
(
  const fvMesh& mesh
)
:
  BasePhaseSystem{mesh}
{
  this->generatePairsAndSubModels
  (
    "drag",
    dragModels_
  );
  this->generatePairsAndSubModels
  (
    "virtualMass",
    virtualMassModels_
  );
  this->generatePairsAndSubModels
  (
    "lift",
    liftModels_
  );
  this->generatePairsAndSubModels
  (
    "wallLubrication",
    wallLubricationModels_
  );
  this->generatePairsAndSubModels
  (
    "turbulentDispersion",
    turbulentDispersionModels_
  );
  FOR_ALL_CONST_ITER
  (
    dragModelTable,
    dragModels_,
    dragModelIter
  )
  {
    const phasePair& pair = this->phasePairs_[dragModelIter.key()];
    Kds_.insert
    (
      pair,
      new volScalarField
      {
        IOobject::groupName("Kd", pair.name()),
        dragModelIter()->K()
      }
    );
  }
  FOR_ALL_CONST_ITER
  (
    virtualMassModelTable,
    virtualMassModels_,
    virtualMassModelIter
  )
  {
    const phasePair& pair = this->phasePairs_[virtualMassModelIter.key()];
    Vms_.insert
    (
      pair,
      new volScalarField
      {
        IOobject::groupName("Vm", pair.name()),
        virtualMassModelIter()->K()
      }
    );
  }
}
// Destructor 
template<class BasePhaseSystem>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::
~MomentumTransferPhaseSystem()
{}
// Member Functions 
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Kd
(
  const phasePairKey& key
) const
{
  return dragModels_[key]->K();
}
template<class BasePhaseSystem>
mousse::tmp<mousse::surfaceScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Kdf
(
  const phasePairKey& key
) const
{
  return dragModels_[key]->Kf();
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Kd
(
  const mousse::phaseModel& phase
) const
{
  tmp<volScalarField> tKd
  {
    new volScalarField
    {
      {
        IOobject::groupName("Kd", phase.name()),
        this->mesh_.time().timeName(),
        this->mesh_
      },
      this->mesh_,
      {IOobject::groupName("Kd", phase.name()), {1, -3, -1, 0, 0}, 0}
    }
  };
  FOR_ALL_CONST_ITER
  (
    phaseSystem::KdTable,
    Kds_,
    KdIter
  )
  {
    const volScalarField& K = *KdIter();
    const phasePair& pair = this->phasePairs_[KdIter.key()];
    const phaseModel* phase1 = &pair.phase1();
    const phaseModel* phase2 = &pair.phase2();
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      if (phase1 == &phase)
      {
        tKd() += K;
      }
      Swap(phase1, phase2);
    }
  }
  return tKd;
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Vm
(
  const phasePairKey& key
) const
{
  if (virtualMassModels_.found(key))
  {
    return virtualMassModels_[key]->K();
  }
  else
  {
    return tmp<volScalarField>
    {
      new volScalarField
      {
        {
          virtualMassModel::typeName + ":K",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", virtualMassModel::dimK, 0}
      }
    };
  }
}
template<class BasePhaseSystem>
mousse::tmp<mousse::surfaceScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Vmf
(
  const phasePairKey& key
) const
{
  if (virtualMassModels_.found(key))
  {
    return virtualMassModels_[key]->Kf();
  }
  else
  {
    return tmp<surfaceScalarField>
    {
      new surfaceScalarField
      {
        {
          virtualMassModel::typeName + ":Kf",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", virtualMassModel::dimK, 0}
      }
    };
  }
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volVectorField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::F
(
  const phasePairKey& key
) const
{
  if (liftModels_.found(key) && wallLubricationModels_.found(key))
  {
    return
      liftModels_[key]->template F<vector>()
     + wallLubricationModels_[key]->template F<vector>();
  }
  else if (liftModels_.found(key))
  {
    return liftModels_[key]->template F<vector>();
  }
  else if (wallLubricationModels_.found(key))
  {
    return wallLubricationModels_[key]->template F<vector>();
  }
  else
  {
    return tmp<volVectorField>
    {
      new volVectorField
      {
        {
          liftModel::typeName + ":F",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", liftModel::dimF, vector::zero}
      }
    };
  }
}
template<class BasePhaseSystem>
mousse::tmp<mousse::surfaceScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Ff
(
  const phasePairKey& key
) const
{
  if (liftModels_.found(key) && wallLubricationModels_.found(key))
  {
    return liftModels_[key]->Ff() + wallLubricationModels_[key]->Ff();
  }
  else if (liftModels_.found(key))
  {
    return liftModels_[key]->Ff();
  }
  else if (wallLubricationModels_.found(key))
  {
    return wallLubricationModels_[key]->Ff();
  }
  else
  {
    return tmp<surfaceScalarField>
    {
      new surfaceScalarField
      {
        {
          liftModel::typeName + ":Ff",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", liftModel::dimF*dimArea, 0}
      }
    };
  }
}
template<class BasePhaseSystem>
mousse::tmp<mousse::volScalarField>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::D
(
  const phasePairKey& key
) const
{
  if (turbulentDispersionModels_.found(key))
  {
    return turbulentDispersionModels_[key]->D();
  }
  else
  {
    return tmp<volScalarField>
    {
      new volScalarField
      {
        {
          turbulentDispersionModel::typeName + ":D",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", turbulentDispersionModel::dimD, 0}
      }
    };
  }
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::phaseSystem::momentumTransferTable>
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::momentumTransfer() const
{
  // Create a momentum transfer matrix for each phase
  autoPtr<phaseSystem::momentumTransferTable> eqnsPtr
  {
    new phaseSystem::momentumTransferTable()
  };
  phaseSystem::momentumTransferTable& eqns = eqnsPtr();
  FOR_ALL(this->phaseModels_, phasei)
  {
    const phaseModel& phase = this->phaseModels_[phasei];
    eqns.insert
    (
      phase.name(),
      new fvVectorMatrix{phase.U(), dimMass*dimVelocity/dimTime}
    );
  }
  // Update the drag coefficients
  FOR_ALL_CONST_ITER
  (
    dragModelTable,
    dragModels_,
    dragModelIter
  )
  {
    *Kds_[dragModelIter.key()] = dragModelIter()->K();
  }
  // Add the implicit part of the drag force
  FOR_ALL_CONST_ITER
  (
    phaseSystem::KdTable,
    Kds_,
    KdIter
  )
  {
    const volScalarField& K = *KdIter();
    const phasePair& pair = this->phasePairs_[KdIter.key()];
    const phaseModel* phase = &pair.phase1();
    const phaseModel* otherPhase = &pair.phase2();
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      const volVectorField& U = phase->U();
      *eqns[phase->name()] -= fvm::Sp(K, U);
      Swap(phase, otherPhase);
    }
  }
  // Update the virtual mass coefficients
  FOR_ALL_CONST_ITER
  (
    virtualMassModelTable,
    virtualMassModels_,
    virtualMassModelIter
  )
  {
    *Vms_[virtualMassModelIter.key()] = virtualMassModelIter()->K();
  }
  // Add the virtual mass force
  FOR_ALL_CONST_ITER
  (
    phaseSystem::VmTable,
    Vms_,
    VmIter
  )
  {
    const volScalarField& Vm = *VmIter();
    const phasePair& pair = this->phasePairs_[VmIter.key()];
    const phaseModel* phase = &pair.phase1();
    const phaseModel* otherPhase = &pair.phase2();
    FOR_ALL_CONST_ITER(phasePair, pair, iter)
    {
      const volVectorField& U = phase->U();
      const surfaceScalarField& phi = phase->phi();
      *eqns[phase->name()] -=
        Vm*(fvm::ddt(U)
            + fvm::div(phi, U)
            - fvm::Sp(fvc::div(phi), U)
            - otherPhase->DUDt())
        + this->MRF_.DDt(Vm, U - otherPhase->U());
      Swap(phase, otherPhase);
    }
  }
  return eqnsPtr;
}
template<class BasePhaseSystem>
mousse::volVectorField& mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::setF
(
  PtrList<volVectorField>& Fs, const label phasei
) const
{
  if (!Fs.set(phasei))
  {
    Fs.set
    (
      phasei,
      new volVectorField
      {
        {
          liftModel::typeName + ":F",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", liftModel::dimF, vector::zero}
      }
    );
  }
  return Fs[phasei];
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::PtrList<mousse::volVectorField> >
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::Fs() const
{
  autoPtr<PtrList<volVectorField> > tFs
  {
    new PtrList<volVectorField>(this->phases().size())
  };
  PtrList<volVectorField>& Fs = tFs();
  // Add the lift force
  FOR_ALL_CONST_ITER
  (
    liftModelTable,
    liftModels_,
    liftModelIter
  )
  {
    const volVectorField F{liftModelIter()->F<vector>()};
    const phasePair& pair = this->phasePairs_[liftModelIter.key()];
    setF(Fs, pair.phase1().index()) += F;
    setF(Fs, pair.phase2().index()) -= F;
  }
  // Add the wall lubrication force
  FOR_ALL_CONST_ITER
  (
    wallLubricationModelTable,
    wallLubricationModels_,
    wallLubricationModelIter
  )
  {
    const volVectorField F{wallLubricationModelIter()->F<vector>()};
    const phasePair& pair = this->phasePairs_[wallLubricationModelIter.key()];
    setF(Fs, pair.phase1().index()) += F;
    setF(Fs, pair.phase2().index()) -= F;
  }
  return tFs;
}
template<class BasePhaseSystem>
mousse::surfaceScalarField&
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::setPhiD
(
  PtrList<surfaceScalarField>& phiDs, const label phasei
) const
{
  if (!phiDs.set(phasei))
  {
    phiDs.set
    (
      phasei,
      new surfaceScalarField
      {
        {
          turbulentDispersionModel::typeName + ":phiD",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", dimTime*dimArea*turbulentDispersionModel::dimF/dimDensity, 0}
      }
    );
  }
  return phiDs[phasei];
}
template<class BasePhaseSystem>
mousse::autoPtr<mousse::PtrList<mousse::surfaceScalarField> >
mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::phiDs
(
  const PtrList<volScalarField>& rAUs
) const
{
  autoPtr<PtrList<surfaceScalarField>> tphiDs
  {
    new PtrList<surfaceScalarField>(this->phases().size())
  };
  PtrList<surfaceScalarField>& phiDs = tphiDs();
  // Add the turbulent dispersion force
  FOR_ALL_CONST_ITER
  (
    turbulentDispersionModelTable,
    turbulentDispersionModels_,
    turbulentDispersionModelIter
  )
  {
    const phasePair& pair =
      this->phasePairs_[turbulentDispersionModelIter.key()];
    const volScalarField D{turbulentDispersionModelIter()->D()};
    const surfaceScalarField snGradAlpha1
    {
      fvc::snGrad(pair.phase1())*this->mesh_.magSf()
    };
    setPhiD(phiDs, pair.phase1().index()) +=
      fvc::interpolate(rAUs[pair.phase1().index()]*D)*snGradAlpha1;
    setPhiD(phiDs, pair.phase2().index()) -=
      fvc::interpolate(rAUs[pair.phase2().index()]*D)*snGradAlpha1;
  }
  return tphiDs;
}
template<class BasePhaseSystem>
bool mousse::MomentumTransferPhaseSystem<BasePhaseSystem>::read()
{
  if (BasePhaseSystem::read())
  {
    bool readOK = true;
    // Read models ...
    return readOK;
  }
  else
  {
    return false;
  }
}
