// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeedModels::constant
// Description
//   Constant laminar flame speed model.
// SourceFiles
//   select_laminar_flame_speed.cpp
#ifndef constant_hpp_
#define constant_hpp_
#include "laminar_flame_speed.hpp"
namespace mousse
{
namespace laminarFlameSpeedModels
{
class constant
:
  public laminarFlameSpeed
{
private:
  // Const laminar flame speed
    dimensionedScalar Su_;
  // Private Member Functions
    //- Construct as copy (not implemented)
    constant(const constant&);
    void operator=(const constant&);
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
  //- Destructor
  virtual ~constant();
  // Member functions
    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;
};
} // End laminarFlameSpeedModels
}  // namespace mousse
#endif
