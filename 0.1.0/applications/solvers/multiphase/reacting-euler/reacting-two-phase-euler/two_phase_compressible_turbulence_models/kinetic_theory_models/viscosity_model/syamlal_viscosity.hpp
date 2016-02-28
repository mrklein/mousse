// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::viscosityModels::Syamlal
// Description
// SourceFiles
//   syamlal_viscosity.cpp
#ifndef Syamlal_H
#define Syamlal_H
#include "viscosity_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace viscosityModels
{
class Syamlal
:
  public viscosityModel
{
public:
  //- Runtime type information
  TYPE_NAME("Syamlal");
  // Constructors
    //- Construct from components
    Syamlal(const dictionary& dict);
  //- Destructor
  virtual ~Syamlal();
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
