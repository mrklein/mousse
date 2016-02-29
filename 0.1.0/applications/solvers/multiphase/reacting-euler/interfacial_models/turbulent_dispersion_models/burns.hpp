#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_BURNS_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_BURNS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModels::Burns
// Description
//   Turbulent dispersion model of Burns et al.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.,
//     PhD Thesis
//     April 2013
//   \endverbatim
//   \verbatim
//     "The Favre averaged drag model for turbulent dispersion in Eulerian
//     multi-phase flows"
//     Burns, A.D., Frank, T., Hamill, I., Shi, J.M.,
//     5th international conference on multiphase flow
//     Volume 4, Paper 392, May 2004
//   \endverbatim
// SourceFiles
//   burns.cpp
#include "turbulent_dispersion_model.hpp"
namespace mousse
{
class phasePair;
namespace turbulentDispersionModels
{
class Burns
:
  public turbulentDispersionModel
{
  // Private data
    //- Schmidt number
    const dimensionedScalar sigma_;
    //- Residual phase fraction
    const dimensionedScalar residualAlpha_;
public:
  //- Runtime type information
  TYPE_NAME("Burns");
  // Constructors
    //- Construct from a dictionary and a phase pair
    Burns
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Burns();
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const;
};
}  // namespace turbulentDispersionModels
}  // namespace mousse
#endif
