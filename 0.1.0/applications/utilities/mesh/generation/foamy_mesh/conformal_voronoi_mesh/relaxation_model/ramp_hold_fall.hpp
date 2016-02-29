#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_RELAXATION_MODEL_RAMP_HOLD_FALL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_RELAXATION_MODEL_RAMP_HOLD_FALL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rampHoldFall
// Description
//   Piecewise linear function with a ramp from a start value to a plateaux
//   value, holding at this, then a linear fall to an end value.
// SourceFiles
//   ramp_hold_fall.cpp
#include "relaxation_model.hpp"
namespace mousse
{
class rampHoldFall
:
  public relaxationModel
{
private:
  // Private data
    //- Relaxation coefficient at the start of the ramp
    scalar rampStartRelaxation_;
    //- Relaxation coefficient for the hold portion
    scalar holdRelaxation_;
    //- Relaxation coefficient at the end of the fall
    scalar fallEndRelaxation_;
    //- Fraction through the run where the ramp ends and the hold starts
    scalar rampEndFraction_;
    //- Fraction through the run where the hold ends and the fall starts
    scalar fallStartFraction_;
    //- Gradient in ramp portion, normalised against time
    scalar rampGradient_;
    //- Gradient in fall portion, normalised against time
    scalar fallGradient_;
public:
  //- Runtime type information
  TYPE_NAME("rampHoldFall");
  // Constructors
    //- Construct from components
    rampHoldFall
    (
      const dictionary& relaxationDict,
      const Time& runTime
    );
  //- Destructor
  virtual ~rampHoldFall()
  {}
  // Member Functions
    //- Return the current relaxation coefficient
    virtual scalar relaxation();
};
}  // namespace mousse
#endif
