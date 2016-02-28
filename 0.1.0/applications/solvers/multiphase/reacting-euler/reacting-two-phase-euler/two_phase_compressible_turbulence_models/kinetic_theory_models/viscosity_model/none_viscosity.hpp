// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noneViscosity
// Description
// SourceFiles
//   none_viscosity.cpp
#ifndef none_viscosity_hpp_
#define none_viscosity_hpp_
#include "viscosity_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class noneViscosity
:
  public viscosityModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noneViscosity(const dictionary& dict);
  //- Destructor
  virtual ~noneViscosity();
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
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
