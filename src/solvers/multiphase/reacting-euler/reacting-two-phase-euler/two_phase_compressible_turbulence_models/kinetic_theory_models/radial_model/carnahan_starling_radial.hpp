// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::radialModels::CarnahanStarling
// Description
// SourceFiles
//   carnahan_starling_radial.cpp
#ifndef CarnahanStarling_H
#define CarnahanStarling_H
#include "radial_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace radialModels
{
class CarnahanStarling
:
  public radialModel
{
public:
  //- Runtime type information
  TYPE_NAME("CarnahanStarling");
  // Constructors
    //- Construct from components
    CarnahanStarling(const dictionary& dict);
  //- Destructor
  virtual ~CarnahanStarling();
  // Member Functions
    tmp<volScalarField> g0
    (
      const volScalarField& alpha,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
    tmp<volScalarField> g0prime
    (
      const volScalarField& alpha,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
};
}  // namespace radialModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
