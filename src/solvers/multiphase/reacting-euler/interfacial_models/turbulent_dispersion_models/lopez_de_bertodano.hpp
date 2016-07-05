#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_LOPEZ_DE_BERTODANO_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_TURBULENT_DISPERSION_MODELS_LOPEZ_DE_BERTODANO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModels::LopezDeBertodano
// Description
//   Lopez de Bertodano (1992) turbulent dispersion model.
//   \verbatim
//     "Turbulent bubbly two-phase flow in a triangular
//     duct"
//     Lopez de Bertodano, M.
//     Ph.D. Thesis, Rensselaer Polytechnic Institution, New York, USA, 1992.
//   \endverbatim
//   \verbatim
//     "The Favre averaged drag model for turbulent dispersion in Eulerian
//     multi-phase flows"
//     Burns, A.D., Frank, T., Hamill, I., Shi, J.M.,
//     5th international conference on multiphase flow
//     Volume 4, Paper 392, May 2004
//   \endverbatim
// SourceFiles
//   lopez_de_bertodano.cpp
#include "turbulent_dispersion_model.hpp"
namespace mousse
{
class phasePair;
namespace turbulentDispersionModels
{
class LopezDeBertodano
:
  public turbulentDispersionModel
{
  // Private data
    //- Constant turbulent dispersion coefficient
    const dimensionedScalar Ctd_;
public:
  //- Runtime type information
  TYPE_NAME("LopezDeBertodano");
  // Constructors
    //- Construct from a dictionary and a phase pair
    LopezDeBertodano
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~LopezDeBertodano();
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const;
};
}  // namespace turbulentDispersionModels
}  // namespace mousse
#endif
