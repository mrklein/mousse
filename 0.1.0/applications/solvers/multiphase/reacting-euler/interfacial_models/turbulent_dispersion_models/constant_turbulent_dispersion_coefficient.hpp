#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_CONSTANT_TURBULENT_DISPERSION_COEFFICIENT_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_CONSTANT_TURBULENT_DISPERSION_COEFFICIENT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModels::constantTurbulentDispersionCoefficient
// Description
//   Constant coefficient turbulent dispersion model.
// SourceFiles
//   constant_turbulent_dispersion_coefficient.cpp
#include "turbulent_dispersion_model.hpp"
namespace mousse
{
class phasePair;
namespace turbulentDispersionModels
{
class constantTurbulentDispersionCoefficient
:
  public turbulentDispersionModel
{
  // Private data
    //- Constant turbulent dispersion coefficient
    const dimensionedScalar Ctd_;
public:
  //- Runtime type information
  TYPE_NAME("constantCoefficient");
  // Constructors
    //- Construct from a dictionary and a phase pair
    constantTurbulentDispersionCoefficient
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~constantTurbulentDispersionCoefficient();
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const;
};
}  // namespace turbulentDispersionModels
}  // namespace mousse
#endif
