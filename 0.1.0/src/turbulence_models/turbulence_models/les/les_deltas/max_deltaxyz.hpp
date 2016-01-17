// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::maxDeltaxyz
// Description
//   Delta calculated by taking the maximum distance between the cell centre
//   and any face centre.  For a regular hex cell, the computed delta will
//   equate to half of the cell width; accordingly, the deltaCoeff model
//   coefficient should be set to 2 for this case.
// SourceFiles
//   max_deltaxyz.cpp
#ifndef maxDeltaxyzDelta_H
#define maxDeltaxyzDelta_H
#include "les_delta.hpp"
namespace mousse
{
namespace LESModels
{
class maxDeltaxyz
:
  public LESdelta
{
  // Private data
    //- Model coefficient
    scalar deltaCoeff_;
  // Private Member Functions
    // Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TYPE_NAME("maxDeltaxyz");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    maxDeltaxyz
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
    //- Disallow default bitwise copy construct and assignment
    maxDeltaxyz(const maxDeltaxyz&) = delete;
    maxDeltaxyz& operator=(const maxDeltaxyz&) = delete;
  //- Destructor
  virtual ~maxDeltaxyz()
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
