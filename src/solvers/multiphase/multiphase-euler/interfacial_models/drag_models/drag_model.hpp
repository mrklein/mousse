// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModel
// Description
// SourceFiles
//   drag_model.cpp
//   new_drag_model.cpp
#ifndef DRAG_MODEL_HPP_
#define DRAG_MODEL_HPP_
#include "dictionary.hpp"
#include "phase_model.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class dragModel
{
protected:
  // Protected data
    const dictionary& interfaceDict_;
    const phaseModel& phase1_;
    const phaseModel& phase2_;
    dimensionedScalar residualPhaseFraction_;
    dimensionedScalar residualSlip_;
public:
  //- Runtime type information
  TYPE_NAME("dragModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      dragModel,
      dictionary,
      (
        const dictionary& interfaceDict,
        const phaseModel& phase1,
        const phaseModel& phase2
      ),
      (interfaceDict, phase1, phase2)
    );
  // Constructors
    dragModel
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~dragModel();
  // Selectors
    static autoPtr<dragModel> New
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  // Member Functions
    const phaseModel& phase1() const
    {
      return phase1_;
    }
    const phaseModel& phase2() const
    {
      return phase2_;
    }
    const dimensionedScalar& residualPhaseFraction() const
    {
      return residualPhaseFraction_;
    }
    const dimensionedScalar& residualSlip() const
    {
      return residualSlip_;
    }
    //- The dragfunction K used in the momentum eq.
    //    ddt(alpha1*rho1*U1) + ... = ... alpha1*alpha2*K*(U1-U2)
    //    ddt(alpha2*rho2*U2) + ...  = ... alpha1*alpha2*K*(U2-U1)
    // ********************************** NB! *****************************
    // for numerical reasons alpha1 and alpha2 has been
    // extracted from the dragFunction K,
    // so you MUST divide K by alpha1*alpha2 when implemnting the drag
    // function
    // ********************************** NB! *****************************
    virtual tmp<volScalarField> K(const volScalarField& Ur) const = 0;
};
}  // namespace mousse
#endif
