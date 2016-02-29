#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_ASPECT_RATIO_MODELS_TOMIYAMA_ASPECT_RATIO_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_ASPECT_RATIO_MODELS_TOMIYAMA_ASPECT_RATIO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::aspectRatioModels::TomiyamaAspectRatio
// Description
//   Aspect ratio model of Tomiyama.
//   Reference:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.,
//     PhD Thesis, April 2013
//   \endverbatim
// SourceFiles
//   tomiyama_aspect_ratio.cpp
#include "vakhrushev_efremov.hpp"
#include "wall_dependent_model.hpp"
namespace mousse
{
namespace aspectRatioModels
{
class TomiyamaAspectRatio
:
  public VakhrushevEfremov,
  public wallDependentModel
{
public:
  //- Runtime type information
  TYPE_NAME("Tomiyama");
  // Constructors
    //- Construct from a dictionary and an ordered phase pair
    TomiyamaAspectRatio
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~TomiyamaAspectRatio();
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
};
}  // namespace aspectRatioModels
}  // namespace mousse
#endif
