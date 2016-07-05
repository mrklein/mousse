// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constant
// Description
//   Constant dispersed-phase particle diameter model.
// SourceFiles
//   constant.cpp
#ifndef CONSTANT_HPP_
#define CONSTANT_HPP_
#include "diameter_model.hpp"
namespace mousse {
namespace diameterModels {
class constant
:
  public diameterModel
{
  // Private data
    //- The constant diameter of the phase
    dimensionedScalar d_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from components
    constant
    (
      const dictionary& dict,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~constant();
  // Member Functions
    tmp<volScalarField> d() const;
};
}  // namespace diameterModels
}  // namespace mousse
#endif
