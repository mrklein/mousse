// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::radialModels::LunSavage
// Description
// SourceFiles
//   lun_savage.cpp
#ifndef LunSavage_H
#define LunSavage_H
#include "radial_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace radialModels
{
class LunSavage
:
  public radialModel
{
public:
  //- Runtime type information
  TYPE_NAME("LunSavage");
  // Constructors
    //- Construct from components
    LunSavage(const dictionary& dict);
  //- Destructor
  virtual ~LunSavage();
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
