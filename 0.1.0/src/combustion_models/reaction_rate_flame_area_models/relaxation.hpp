#ifndef COMBUSTION_MODELS_REACTION_RATE_FLAME_AREA_MODELS_RELAXATION_HPP_
#define COMBUSTION_MODELS_REACTION_RATE_FLAME_AREA_MODELS_RELAXATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactionRateFlameAreaModels::relaxation
// Description
//   Consumption rate per unit of flame area obtained from a relaxation equation
// SourceFiles
//   relaxation.cpp
#include "reaction_rate_flame_area.hpp"
#include "consumption_speed.hpp"
namespace mousse
{
namespace reactionRateFlameAreaModels
{
class relaxation
:
  public reactionRateFlameArea
{
  // Private Data
    //- Correlation
    consumptionSpeed correlation_;
    //- Proportionality constant for time scale in the relaxation Eq.
    scalar C_;
    //- Proportionality constant for sub-grid strain
    scalar alpha_;
public:
  //- Runtime type information
  TYPE_NAME("relaxation");
  // Constructors
    //- Construct from dictionary and psiReactionThermo
    relaxation
    (
      const word modelType,
      const dictionary& dictCoeffs,
      const fvMesh& mesh,
      const combustionModel& combModel
    );
    //- Disallow copy construct
    relaxation(const relaxation&) = delete;
    //- Disallow default bitwise assignment
    relaxation& operator=(const relaxation&) = delete;
  // Destructor
    virtual ~relaxation();
  // Member functions
    //- Correct omega
    virtual void correct(const volScalarField& sigma);
  // IO
    //- Update properties from given dictionary
    virtual bool read(const dictionary& dictProperties);
};
} // End reactionRateFlameAreaModels
}  // namespace mousse
#endif
