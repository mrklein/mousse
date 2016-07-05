#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_HEAT_AND_MASS_TRANSFER_PHASE_SYSTEM_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_HEAT_AND_MASS_TRANSFER_PHASE_SYSTEM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::HeatAndMassTransferPhaseSystem
// Description
//   Base class to support interfacial heat and mass transfer between a number
//   of phases.
// SourceFiles
//   heat_and_mass_transfer_phase_system.cpp
#include "phase_system.hpp"
namespace mousse
{
template <class modelType>
class BlendedInterfacialModel;
class blendingMethod;
class heatTransferModel;
class massTransferModel;
template<class BasePhaseSystem>
class HeatAndMassTransferPhaseSystem
:
  public BasePhaseSystem
{
protected:
  // Protected typedefs
    typedef HashTable
    <
      HashTable
      <
        autoPtr<BlendedInterfacialModel<heatTransferModel> >
      >,
      phasePairKey,
      phasePairKey::hash
    > heatTransferModelTable;
    typedef HashTable
    <
      HashTable
      <
        autoPtr<BlendedInterfacialModel<massTransferModel> >
      >,
      phasePairKey,
      phasePairKey::hash
    > massTransferModelTable;
  // Protected data
    //- Mass transfer rate
    HashPtrTable<volScalarField, phasePairKey, phasePairKey::hash>
      dmdt_;
    //- Explicit part of the mass transfer rate
    HashPtrTable<volScalarField, phasePairKey, phasePairKey::hash>
      dmdtExplicit_;
    //- Interface temperatures
    HashPtrTable<volScalarField, phasePairKey, phasePairKey::hash> Tf_;
    // Sub Models
      //- Heat transfer models
      heatTransferModelTable heatTransferModels_;
      //- Mass transfer models
      massTransferModelTable massTransferModels_;
public:
  // Constructors
    //- Construct from fvMesh
    HeatAndMassTransferPhaseSystem(const fvMesh&);
  //- Destructor
  virtual ~HeatAndMassTransferPhaseSystem();
  // Member Functions
    //- Return true if there is mass transfer for phase
    virtual bool transfersMass(const phaseModel& phase) const;
    //- Return the interfacial mass flow rate
    virtual tmp<volScalarField> dmdt(const phasePairKey& key) const;
    //- Return the total interfacial mass transfer rate for phase
    virtual tmp<volScalarField> dmdt(const phaseModel& phase) const;
    //- Return the momentum transfer matrices
    virtual autoPtr<phaseSystem::momentumTransferTable>
      momentumTransfer() const;
    //- Return the heat transfer matrices
    virtual autoPtr<phaseSystem::heatTransferTable>
      heatTransfer() const;
    //- Return the mass transfer matrices
    virtual autoPtr<phaseSystem::massTransferTable>
      massTransfer() const = 0;
    //- Correct the thermodynamics
    virtual void correctThermo() = 0;
    //- Read base phaseProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#ifdef NoRepository
#   include "heat_and_mass_transfer_phase_system.cpp"
#endif
#endif
