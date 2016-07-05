// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModel
// Description
// SourceFiles
//   heat_transfer_model.cpp
//   new_heat_transfer_model.cpp
#ifndef HEAT_TRANSFER_MODEL_HPP_
#define HEAT_TRANSFER_MODEL_HPP_
#include "dictionary.hpp"
#include "phase_model.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class heatTransferModel
{
protected:
  // Protected data
    const dictionary& interfaceDict_;
    const volScalarField& alpha1_;
    const phaseModel& phase1_;
    const phaseModel& phase2_;
public:
  //- Runtime type information
  TYPE_NAME("heatTransferModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      heatTransferModel,
      dictionary,
      (
        const dictionary& interfaceDict,
        const volScalarField& alpha1,
        const phaseModel& phase1,
        const phaseModel& phase2
      ),
      (interfaceDict, alpha1, phase1, phase2)
    );
  // Constructors
    heatTransferModel
    (
      const dictionary& interfaceDict,
      const volScalarField& alpha1,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~heatTransferModel();
  // Selectors
    static autoPtr<heatTransferModel> New
    (
      const dictionary& interfaceDict,
      const volScalarField& alpha1,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  // Member Functions
    //- The heat-transfer function K used in the enthalpy eq.
    //    ddt(alpha1*rho1*ha) + ... = ... alpha1*alpha2*K*(Ta - Tb)
    //    ddt(alpha2*rho2*hb) + ...  = ... alpha1*alpha2*K*(Tb - Ta)
    // ********************************** NB! *****************************
    // for numerical reasons alpha1 and alpha2 has been
    // extracted from the heat-transfer function K,
    // so you MUST divide K by alpha1*alpha2 when implementing the
    // heat-transfer function
    // ********************************** NB! *****************************
    virtual tmp<volScalarField> K(const volScalarField& Ur) const = 0;
};
}  // namespace mousse
#endif
