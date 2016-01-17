// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adaptiveLinear
// Description
//   Produces a linear ramp which adapts its gradient to changes in
//   endTime and deltaT to always arrive at the relaxationEnd value at the end of
//   the run
// SourceFiles
//   adaptive_linear.cpp
#ifndef adaptive_linear_hpp_
#define adaptive_linear_hpp_
#include "relaxation_model.hpp"
namespace mousse
{
class adaptiveLinear
:
  public relaxationModel
{
private:
  // Private data
    //- Relaxation coefficient at the start of the iteration sequence.
    scalar relaxationStart_;
    //- Relaxation coefficient at the end of the iteration sequence.
    scalar relaxationEnd_;
    //- Store the time when the last request was made for relaxation,
    //  prevents multiple calls to relaxation in a timestep from
    //  incrementing the value
    scalar lastTimeValue_;
    //- Current relaxation value
    scalar relaxation_;
public:
  //- Runtime type information
  TypeName("adaptiveLinear");
  // Constructors
    //- Construct from components
    adaptiveLinear
    (
      const dictionary& relaxationDict,
      const Time& runTime
    );
  //- Destructor
  virtual ~adaptiveLinear()
  {}
  // Member Functions
    //- Return the current relaxation coefficient
    virtual scalar relaxation();
};
}  // namespace mousse
#endif
