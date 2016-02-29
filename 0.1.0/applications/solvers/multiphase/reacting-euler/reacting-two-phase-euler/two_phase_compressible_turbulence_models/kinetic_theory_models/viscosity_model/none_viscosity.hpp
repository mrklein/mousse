#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_VISCOSITY_MODEL_NONE_VISCOSITY_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_VISCOSITY_MODEL_NONE_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noneViscosity
// Description
// SourceFiles
//   none_viscosity.cpp
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
