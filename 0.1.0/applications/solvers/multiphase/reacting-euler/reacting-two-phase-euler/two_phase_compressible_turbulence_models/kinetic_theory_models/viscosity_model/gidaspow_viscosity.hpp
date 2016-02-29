// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::viscosityModels::Gidaspow
// Description
// SourceFiles
//   gidaspow_viscosity.cpp
#ifndef Gidaspow_H
#define Gidaspow_H
#include "viscosity_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace viscosityModels
{
class Gidaspow
:
  public viscosityModel
{
public:
  //- Runtime type information
  TYPE_NAME("Gidaspow");
  // Constructors
    //- Construct from components
    Gidaspow(const dictionary& dict);
  //- Destructor
  virtual ~Gidaspow();
  // Member functions
    tmp<volScalarField> nu
    (
      const volScalarField& alpha1,
      const volScalarField& Theta,
      const volScalarField& g0,
      const volScalarField& rho1,
      const volScalarField& da,
      const dimensionedScalar& e
    ) const;
};
}  // namespace viscosityModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
