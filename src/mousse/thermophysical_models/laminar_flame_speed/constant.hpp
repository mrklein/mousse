#ifndef THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_CONSTANT_HPP_
#define THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_CONSTANT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeedModels::constant
// Description
//   Constant laminar flame speed model.

#include "laminar_flame_speed.hpp"


namespace mousse {
namespace laminarFlameSpeedModels {

class constant
:
  public laminarFlameSpeed
{
private:
  // Const laminar flame speed
    dimensionedScalar Su_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from dictionary and psiuReactionThermo
    constant
    (
      const dictionary&,
      const psiuReactionThermo&
    );
    constant(const constant&) = delete;
    void operator=(const constant&) = delete;
  //- Destructor
  virtual ~constant();
  // Member functions
    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;
};

}  // namespace laminarFlameSpeedModels
}  // namespace mousse

#endif
