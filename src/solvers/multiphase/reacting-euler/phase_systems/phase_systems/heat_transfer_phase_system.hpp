#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_HEAT_TRANSFER_PHASE_SYSTEM_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_SYSTEMS_HEAT_TRANSFER_PHASE_SYSTEM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::HeatTransferPhaseSystem
// Description
//   Class which models interfacial heat transfer between a number of phases.
// SourceFiles
//   heat_transfer_phase_system.cpp
#include "phase_system.hpp"
namespace mousse
{
class blendingMethod;
template <class modelType> class BlendedInterfacialModel;
class heatTransferModel;
template<class BasePhaseSystem>
class HeatTransferPhaseSystem
:
  public BasePhaseSystem
{
protected:
  // Protected typedefs
    typedef HashTable
    <
      autoPtr<BlendedInterfacialModel<heatTransferModel> >,
      phasePairKey,
      phasePairKey::hash
    > heatTransferModelTable;
  // Protected data
    // Sub Models
      //- Heat transfer models
      heatTransferModelTable heatTransferModels_;
public:
  // Constructors
    //- Construct from fvMesh
    HeatTransferPhaseSystem(const fvMesh&);
  //- Destructor
  virtual ~HeatTransferPhaseSystem();
  // Member Functions
    //- Return true if there is mass transfer for phase
    virtual bool transfersMass(const phaseModel& phase) const;
    //- Return the interfacial mass flow rate
    virtual tmp<volScalarField> dmdt
    (
      const phasePairKey& key
    ) const;
    //- Return the total interfacial mass transfer rate for phase
    virtual tmp<volScalarField> dmdt(const phaseModel& phase) const;
    //- Return the heat transfer matrices
    virtual autoPtr<phaseSystem::heatTransferTable>
      heatTransfer() const;
    //- Return the mass transfer matrices
    virtual autoPtr<phaseSystem::massTransferTable>
      massTransfer() const;
    //- Read base phaseProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#ifdef NoRepository
#   include "heat_transfer_phase_system.cpp"
#endif
#endif
