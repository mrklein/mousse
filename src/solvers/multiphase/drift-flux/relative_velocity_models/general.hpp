#ifndef GENERAL_HPP_
#define GENERAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::general
// Description
//   General relative velocity model

#include "relative_velocity_model.hpp"


namespace mousse {
namespace relativeVelocityModels {

class general
:
  public relativeVelocityModel
{
  // Private data
    //- A coefficient
    dimensionedScalar a_;
    //- a1 coefficient
    dimensionedScalar a1_;
    //- Drift velocity
    dimensionedVector V0_;
    //- Residual phase fraction
    dimensionedScalar residualAlpha_;
public:
  //- Runtime type information
  TYPE_NAME("general");
  // Constructors
    //- Construct from components
    general
    (
      const dictionary& dict,
      const incompressibleTwoPhaseInteractingMixture& mixture
    );
  //- Destructor
  ~general();
  // Member Functions
    //- Update the diffusion velocity
    virtual void correct();
};

}  // namespace relativeVelocityModels
}  // namespace mousse

#endif

