// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::conductivityModels::Syamlal
// Description
// SourceFiles
//   syamlal.cpp
#ifndef SYAMLAL_HPP_
#define SYAMLAL_HPP_
#include "conductivity_model.hpp"
namespace mousse {
namespace kineticTheoryModels {
namespace conductivityModels {
class Syamlal
:
  public conductivityModel
{
public:
  //- Runtime type information
  TYPE_NAME("Syamlal");
  // Constructors
    //- Construct from components
    Syamlal(const dictionary& dict);
  //- Destructor
  virtual ~Syamlal();
  // Member Functions
    tmp<volScalarField> kappa
    (
      const volScalarField& alpha1,
      const volScalarField& Theta,
      const volScalarField& g0,
      const volScalarField& rho1,
      const volScalarField& da,
      const dimensionedScalar& e
    ) const;
};
}  // namespace conductivityModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
