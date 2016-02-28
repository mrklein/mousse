// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SinclairJackson
// Description
// SourceFiles
//   sinclair_jackson_radial.cpp
#ifndef SinclairJackson_H
#define SinclairJackson_H
#include "radial_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace radialModels
{
class SinclairJackson
:
  public radialModel
{
public:
  //- Runtime type information
  TYPE_NAME("SinclairJackson");
  // Constructors
    //- Construct from components
    SinclairJackson(const dictionary& dict);
  //- Destructor
  virtual ~SinclairJackson();
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
