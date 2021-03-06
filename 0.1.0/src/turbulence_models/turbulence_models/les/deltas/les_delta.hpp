#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_DELTAS_LES_DELTA_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_DELTAS_LES_DELTA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESdelta
// Description
//   Abstract base class for LES deltas

#include "turbulence_model.hpp"
#include "vol_fields.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class LESdelta
{
protected:
  // Protected data
    const turbulenceModel& turbulenceModel_;
    volScalarField delta_;
public:
  //- Runtime type information
  TYPE_NAME("LESdelta");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      LESdelta,
      dictionary,
      (
        const word& name,
        const turbulenceModel& turbulence,
        const dictionary& dict
      ),
      (name, turbulence, dict)
    );
    // Disallow default bitwise copy construct and assignment
    LESdelta(const LESdelta&) = delete;
    LESdelta& operator=(const LESdelta&) = delete;
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    LESdelta
    (
      const word& name,
      const turbulenceModel& turbulence
    );
  // Selectors
    //- Return a reference to the selected LES delta
    static autoPtr<LESdelta> New
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary& dict
    );
    //- Return a reference to the selected LES delta
    static autoPtr<LESdelta> New
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary& dict,
      const dictionaryConstructorTable&
    );
  //- Destructor
  virtual ~LESdelta()
  {}
  // Member Functions
    //- Return turbulenceModel reference
    const turbulenceModel& turbulence() const { return turbulenceModel_; }
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&) = 0;
    // Correct values
    virtual void correct() = 0;
  // Member Operators
    virtual operator const volScalarField&() const { return delta_; }
};

}  // namespace mousse

#endif

