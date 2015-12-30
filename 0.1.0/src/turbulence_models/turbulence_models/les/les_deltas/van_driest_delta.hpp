// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vanDriestDelta
// Description
//   Simple cube-root of cell volume delta used in incompressible LES models.
// SourceFiles
//   van_driest_delta.cpp
#ifndef van_driest_delta_hpp_
#define van_driest_delta_hpp_
#include "les_delta.hpp"
namespace mousse
{
namespace LESModels
{
class vanDriestDelta
:
  public LESdelta
{
  // Private data
    autoPtr<LESdelta> geometricDelta_;
    scalar kappa_;
    scalar Aplus_;
    scalar Cdelta_;
    label calcInterval_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    vanDriestDelta(const vanDriestDelta&);
    void operator=(const vanDriestDelta&);
    // Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TypeName("vanDriest");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    vanDriestDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
  //- Destructor
  virtual ~vanDriestDelta()
  {}
  // Member Functions
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&);
    // Correct values
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#endif
