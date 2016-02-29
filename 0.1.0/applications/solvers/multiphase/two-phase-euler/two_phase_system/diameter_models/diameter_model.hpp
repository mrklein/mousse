#ifndef SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_DIAMETER_MODEL_HPP_
#define SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_DIAMETER_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModel
// Description
//   A2stract base-class for dispersed-phase particle diameter models.
// SourceFiles
//   diameter_model.cpp
//   new_diameter_model.cpp
#include "dictionary.hpp"
#include "phase_model.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class diameterModel
{
protected:
  // Protected data
    dictionary diameterProperties_;
    const phaseModel& phase_;
public:
  //- Runtime type information
  TYPE_NAME("diameterModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      diameterModel,
      dictionary,
      (
        const dictionary& diameterProperties,
        const phaseModel& phase
      ),
      (diameterProperties, phase)
    );
  // Constructors
    diameterModel
    (
      const dictionary& diameterProperties,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~diameterModel();
  // Selectors
    static autoPtr<diameterModel> New
    (
      const dictionary& diameterProperties,
      const phaseModel& phase
    );
  // Member Functions
    //- Return the phase diameter properties dictionary
    const dictionary& diameterProperties() const
    {
      return diameterProperties_;
    }
    //- Return the phase
    const phaseModel& phase() const
    {
      return phase_;
    }
    //- Return the phase mean diameter field
    virtual tmp<volScalarField> d() const = 0;
    //- Correct the diameter field
    virtual void correct();
    //- Read phaseProperties dictionary
    virtual bool read(const dictionary& phaseProperties) = 0;
};
}  // namespace mousse
#endif
