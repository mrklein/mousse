// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::isothermal
// Description
//   Isothermal dispersed-phase particle diameter model.
// SourceFiles
//   isothermal.cpp
#ifndef ISOTHERMAL_HPP_
#define ISOTHERMAL_HPP_
#include "diameter_model.hpp"
namespace mousse
{
namespace diameterModels
{
class isothermal
:
  public diameterModel
{
  // Private data
    //- Reference diameter for the isothermal expansion
    dimensionedScalar d0_;
    //- Reference pressure for the isothermal expansion
    dimensionedScalar p0_;
public:
  //- Runtime type information
  TYPE_NAME("isothermal");
  // Constructors
    //- Construct from components
    isothermal
    (
      const dictionary& dict,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~isothermal();
  // Member Functions
    tmp<volScalarField> d() const;
};
}  // namespace diameterModels
}  // namespace mousse
#endif
