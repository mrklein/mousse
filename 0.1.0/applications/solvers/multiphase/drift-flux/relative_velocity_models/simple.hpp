// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simple
// Description
//   Simple relative velocity model
// SourceFiles
//   simple.cpp
#ifndef SIMPLE_HPP_
#define SIMPLE_HPP_
#include "relative_velocity_model.hpp"
namespace mousse
{
namespace relativeVelocityModels
{
class simple
:
  public relativeVelocityModel
{
  // Private data
    //- A coefficient
    dimensionedScalar a_;
    //- Drift velocity
    dimensionedVector V0_;
    //- Residual phase fraction
    dimensionedScalar residualAlpha_;
public:
  //- Runtime type information
  TYPE_NAME("simple");
  // Constructors
    //- Construct from components
    simple
    (
      const dictionary& dict,
      const incompressibleTwoPhaseInteractingMixture& mixture
    );
  //- Destructor
  ~simple();
  // Member Functions
    //- Update the diffusion velocity
    virtual void correct();
};
}  // namespace relativeVelocityModels
}  // namespace mousse
#endif
