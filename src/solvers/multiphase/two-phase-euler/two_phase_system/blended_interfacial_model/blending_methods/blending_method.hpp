// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blendingMethod
// Description
// SourceFiles
//   blending_method.cpp
#ifndef BLENDING_METHOD_HPP_
#define BLENDING_METHOD_HPP_
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
#include "phase_model.hpp"
namespace mousse
{
class blendingMethod
{
public:
  //- Runtime type information
  TYPE_NAME("blendingMethod");
  // Declare runtime construction
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    blendingMethod,
    dictionary,
    (
      const dictionary& dict,
      const wordList& phaseNames
    ),
    (dict, phaseNames)
  );
  // Constructors
    //- Construct from a dictionary
    blendingMethod
    (
      const dictionary& dict
    );
  // Selector
    static autoPtr<blendingMethod> New
    (
      const dictionary& dict,
      const wordList& phaseNames
    );
  //- Destructor
  virtual ~blendingMethod();
  // Member Functions
    //- Factor for first phase
    virtual tmp<volScalarField> f1
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const = 0;
    //- Factor for second phase
    virtual tmp<volScalarField> f2
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const = 0;
};
}  // namespace mousse
#endif
