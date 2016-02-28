// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ThermalPhaseChangePhaseSystem
// Description
//   Class to provide interfacial heat and mass transfer between a number of
//   phases according the interfacial temperature approximated by the saturation
//   temperature.
//   Currently only a single specified specie is considered volatile and changes
//   phase, all other species are considered nonvolatile and do not
//   affect the mass-transfer.
// SourceFiles
//   thermal_phase_change_phase_system.cpp
#ifndef thermal_phase_change_phase_system_hpp_
#define thermal_phase_change_phase_system_hpp_
#include "heat_and_mass_transfer_phase_system.hpp"
#include "saturation_model.hpp"
#include "switch.hpp"
namespace mousse
{
template<class BasePhaseSystem>
class ThermalPhaseChangePhaseSystem
:
  public HeatAndMassTransferPhaseSystem<BasePhaseSystem>
{
protected:
  // Protected data
    //- Name of the volatile specie
    word volatile_;
    //- The saturation model used to evaluate Tsat = Tf
    autoPtr<saturationModel> saturationModel_;
    // Mass transfer enabled
    Switch massTransfer_;
    //- Interfacial Mass transfer rate
    HashPtrTable<volScalarField, phasePairKey, phasePairKey::hash>
      iDmdt_;
    //- Wall Mass transfer rate
    HashPtrTable<volScalarField, phasePairKey, phasePairKey::hash>
      wDmdt_;
public:
  // Constructors
    //- Construct from fvMesh
    ThermalPhaseChangePhaseSystem(const fvMesh&);
  //- Destructor
  virtual ~ThermalPhaseChangePhaseSystem();
  // Member Functions
    //- Return the saturationModel
    const saturationModel& saturation() const;
    //- Return the mass transfer matrices
    virtual autoPtr<phaseSystem::massTransferTable> massTransfer() const;
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Read base phaseProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#ifdef NoRepository
#   include "thermal_phase_change_phase_system.cpp"
#endif
#endif
