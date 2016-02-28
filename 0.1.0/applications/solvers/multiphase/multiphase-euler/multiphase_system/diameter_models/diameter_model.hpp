// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModel
// Description
//   Abstract base-class for dispersed-phase particle diameter models.
// SourceFiles
//   diameter_model.cpp
//   new_diameter_model.cpp
#ifndef DIAMETER_MODEL_HPP_
#define DIAMETER_MODEL_HPP_
#include "dictionary.hpp"
#include "phase_model.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class diameterModel
{
protected:
  // Protected data
    const dictionary& dict_;
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
        const dictionary& dict,
        const phaseModel& phase
      ),
      (dict, phase)
    );
  // Constructors
    diameterModel
    (
      const dictionary& dict,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~diameterModel();
  // Selectors
    static autoPtr<diameterModel> New
    (
      const dictionary& dict,
      const phaseModel& phase
    );
  // Member Functions
    //- Return the phase mean diameter field
    virtual tmp<volScalarField> d() const = 0;
};
}  // namespace mousse
#endif
