#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_INTERFACE_COMPOSITION_PHASE_CHANGE_PHASE_SYSTEM_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_INTERFACE_COMPOSITION_PHASE_CHANGE_PHASE_SYSTEM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InterfaceCompositionPhaseChangePhaseSystem
// Description
//   Class to provide interfacial heat and mass transfer between a number of
//   phases according to a interface composition model.
//   The interface temperature is calculated such that the net rate at which the
//   heat is transferred to the interface is equal to the latent heat consumed by
//   the mass transfer.
// SourceFiles
//   interface_composition_phase_change_phase_system.cpp
#include "heat_and_mass_transfer_phase_system.hpp"
namespace mousse
{
class interfaceCompositionModel;
template<class BasePhaseSystem>
class InterfaceCompositionPhaseChangePhaseSystem
:
  public HeatAndMassTransferPhaseSystem<BasePhaseSystem>
{
protected:
  // Protected typedefs
    typedef HashTable
    <
      autoPtr<interfaceCompositionModel>,
      phasePairKey,
      phasePairKey::hash
    > interfaceCompositionModelTable;
  // Protected data
    // Sub Models
      //- Interface composition models
      interfaceCompositionModelTable interfaceCompositionModels_;
public:
  // Constructors
    //- Construct from fvMesh
    InterfaceCompositionPhaseChangePhaseSystem(const fvMesh&);
  //- Destructor
  virtual ~InterfaceCompositionPhaseChangePhaseSystem();
  // Member Functions
    //- Return the mass transfer matrices
    virtual autoPtr<phaseSystem::massTransferTable> massTransfer() const;
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Read base phaseProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#ifdef NoRepository
#   include "interface_composition_phase_change_phase_system.cpp"
#endif
#endif
