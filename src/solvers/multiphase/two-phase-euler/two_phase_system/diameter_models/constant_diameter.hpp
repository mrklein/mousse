// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::constant
// Description
//   Constant dispersed-phase particle diameter model.
// SourceFiles
//   constant.cpp
#ifndef CONSTANT_H
#define CONSTANT_H
#include "diameter_model.hpp"
namespace mousse
{
namespace diameterModels
{
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
      const dictionary& diameterProperties,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~constant();
  // Member Functions
    //- Return the diameter as a field
    virtual tmp<volScalarField> d() const;
    //- Read diameterProperties dictionary
    virtual bool read(const dictionary& diameterProperties);
};
}  // namespace diameterModels
}  // namespace mousse
#endif
