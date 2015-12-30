// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESdelta
// Description
//   Abstract base class for LES deltas
// SourceFiles
//   les_delta.cpp
#ifndef les_delta_hpp_
#define les_delta_hpp_
#include "turbulence_model.hpp"
#include "vol_fields.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class LESdelta
{
protected:
  // Protected data
    const turbulenceModel& turbulenceModel_;
    volScalarField delta_;
private:
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    LESdelta(const LESdelta&);
    void operator=(const LESdelta&);
public:
  //- Runtime type information
  TypeName("LESdelta");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
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
    const turbulenceModel& turbulence() const
    {
      return turbulenceModel_;
    }
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&) = 0;
    // Correct values
    virtual void correct() = 0;
  // Member Operators
    virtual operator const volScalarField&() const
    {
      return delta_;
    }
};
}  // namespace mousse
#endif
